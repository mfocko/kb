#!/usr/bin/env python3

from typing import Iterable, List, Any
import click

import glob
import json
import os
from pathlib import Path
import subprocess
import tempfile


@click.group()
def cli() -> None:
    pass


class TestCase:
    def __init__(self, target: str, test_case: str, config: Any = None):
        self.target = target
        self.test_case = test_case

        self.test_case_config_path = f"{test_case}.json"
        self._config = config

    @property
    def config(self):
        if not self._config:
            self._config = load_config(self.test_case_config_path)

        return self._config

    def __preprocess(self, arr: List[str]) -> Iterable[str]:
        return map(
            lambda e: e.format(target=self.target, test_case=self.test_case), arr
        )

    def check_stdio(self) -> bool:
        if not self.config.get("capture_stdout"):
            return True

        process = subprocess.run(
            [
                "diff",
                f"{self.test_case}.out",
                f"{self.test_case}.out_produced",
            ],
            capture_output=True,
        )
        if process.returncode == 0:
            return True

        click.secho(
            f"    [FAIL] Output differs from the expected\n\n{process.stdout.decode()}", fg="yellow"
        )
        return False

    def check_specialized_test(self) -> bool:
        manual_test = self.config.get("specialized_test")
        if not manual_test:
            return True

        process = subprocess.run(self.__preprocess(manual_test), capture_output=True)
        if process.returncode == 0:
            return True

        click.secho(
            f"    [FAIL] Specialized test failed\n\n{process.stdout.decode()}\n{process.stderr.decode()}",
            fg="yellow",
        )
        return False

    def check_stderr(self, stderr: tempfile.TemporaryFile) -> None:
        stderr.seek(0)
        if contents := stderr.read():
            click.secho(
                f"    [ LOG] Captured stderr\n\n{contents.decode()}\n",
                fg="yellow"
            )

    def execute_test_case(self) -> bool:
        stdin = None
        stdout = None
        stderr = tempfile.TemporaryFile()
        success = True

        if self.config.get("has_stdin"):
            stdin = open(f"{self.test_case}.in", "r")

        if self.config.get("capture_stdout"):
            stdout = open(f"{self.test_case}.out_produced", "w")

        try:
            self.process = subprocess.run(
                [f"build/{self.target}", *self.__preprocess(self.config["args"])],
                cwd=os.getcwd(),
                stdin=stdin,
                stdout=stdout,
                stderr=stderr,
                timeout=5,
            )
        except subprocess.TimeoutExpired:
            success = False

        self.check_stderr(stderr)
        success = self.check_stdio() and success
        success = self.check_specialized_test() and success

        stderr.close()

        return success


def load_config(path: str) -> Any:
    path_to_config = Path(path)
    if not path_to_config.exists():
        return None

    with open(path_to_config, "r") as config_file:
        return json.load(config_file)


@cli.command()
@click.argument("target")
@click.option("--global-config/--no-global-config", default=True)
def test(target, global_config):
    passed, failed = 0, 0

    config = False
    if global_config:
        config = load_config(f"./{target}.json")

    test_cases = set(
        map(lambda x: str(Path(x).with_suffix("")), glob.glob(f"test-{target}/*"))
    )

    for test_case in test_cases:
        individual_config_exists = Path(f"{test_case}.json").exists()
        if global_config == individual_config_exists:
            continue

        click.secho(f"[TEST] {test_case}")
        if TestCase(target, test_case, config).execute_test_case():
            click.secho("[PASS] Test passed.\n", fg="green")
            passed += 1
        else:
            click.secho("[FAIL] Test failed.\n", fg="red")
            failed += 1

    total = passed + failed
    click.secho(f"Passed: {passed}/{total}\tFailed: {failed}/{total}")


if __name__ == "__main__":
    cli()
