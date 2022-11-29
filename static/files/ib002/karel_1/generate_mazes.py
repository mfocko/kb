#!/usr/bin/env python3

from itertools import product


def generate_map(template: str, st: int, ave: int) -> str:
    return template.format(st=st, ave=ave)


def main():
    template = None
    with open("maze_skel.kw", "r") as f:
        template = f.read()

    for st, ave in product(range(1, 7), range(1, 7)):
        with open("maze{:03d}.kw".format(st * 6 + ave), "w") as out:
            print(generate_map(template, st, ave), file=out)


if __name__ == "__main__":
    main()
