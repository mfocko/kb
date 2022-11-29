from dataclasses import dataclass
from enum import Enum, IntEnum
from functools import wraps
from itertools import product
from math import radians, sin, cos
from sys import stderr
import tkinter as tk
from typing import Tuple, Union


def check_before(precondition):
    """
    Decorator for running methods before executing a method.

    Args:
        call_before: Function to be called before executing a method.

    Returns:
        Parametrized decorator that can be used to wrap method with the
        `call_before`.
    """

    def parametrized(f):
        @wraps(f)
        def wrapper(self, *args, **kwargs):
            if not precondition(self, *args, **kwargs, fn=f):
                return
            return f(self, *args, **kwargs)

        return wrapper

    return parametrized


def after(call_after):
    """
    Decorator for running methods after executing a method.

    Args:
        call_after: Function to be called after executing a method.

    Returns:
        Parametrized decorator that can be used to wrap method with the
        `call_after`.
    """

    def parametrized(f):
        @wraps(f)
        def wrapper(self, *args, **kwargs):
            result = f(self, *args, **kwargs)
            call_after(self, *args, **kwargs, fn=f)
            return result

        return wrapper

    return parametrized


def skip_for_summary(f):
    """
    Decorator used for skipping a function call if the Karel is running
    in a summary mode.
    """

    @wraps(f)
    def wrapper(self, *args, **kwargs):
        if self.summary_mode:
            return
        return f(self, *args, **kwargs)

    return wrapper


class Block(IntEnum):
    """
    Represents a field in the world of the Robot Karel.
    It can be either clear or a wall.
    """

    Clear = 0
    Wall = -1


# Constants for major angles
EAST, NORTH, WEST, SOUTH = 0, 90, 180, 270


# @dataclass(frozen=True, slots=True)
@dataclass(frozen=True)
class _Direction:
    """
    angle: Represents heading of the Robot Karel
    """

    angle: int

    @property
    def vector(self) -> Tuple[int, int]:
        """
        Get a direction vector in the current direction the robot is heading.

        Returns:
            Pair of ints, representing a vector of (dx, dy).
        """
        angle, x, y = radians(self.angle), 1, 0
        return (
            x * round(cos(angle)) - y * round(sin(angle)),
            x * round(sin(angle)) + y * round(cos(angle)),
        )

    @property
    def in_world(self) -> str:
        """
        Get a character that is being put in the graphical representation of
        the Robot Karel.

        Returns:
            Character to be put in the world.
        """
        return {
            Direction.East.value: ">",
            Direction.North.value: "^",
            Direction.West.value: "<",
            Direction.South.value: "v",
            # Direction.East.value: "⮞",
            # Direction.North.value: "⮝",
            # Direction.West.value: "⮜",
            # Direction.South.value: "⮟",
        }[self]

    @property
    def as_string(self) -> str:
        """
        Returns a string with a direction for Tk label.

        Returns:
            Label of the direction
        """
        return {
            Direction.East.value: "east",
            Direction.North.value: "north",
            Direction.West.value: "west",
            Direction.South.value: "south",
        }[self]

    def __add__(self, angle: int) -> "_Direction":
        return _Direction((360 + self.angle + angle) % 360)


class Direction(Enum):
    """
    Provides constants of directions Karel can face.
    """

    East = _Direction(EAST)
    North = _Direction(NORTH)
    West = _Direction(WEST)
    South = _Direction(SOUTH)


class KarelWindow:
    def __init__(self, karel: "Karel", scale: int, font: str):
        self.size = 32 * scale

        self.steps = tk.Label(text=karel.steps, anchor="e")
        self.steps.grid(row=0, column=0, sticky="e")

        self.last_cmd = tk.Label(text=karel.last_command, anchor="w")
        self.last_cmd.grid(row=0, column=1, columnspan=3, sticky="w")

        tk.Label(text="CORNER").grid(row=1, column=0)
        tk.Label(text="FACING").grid(row=1, column=1)
        tk.Label(text="BEEP-BAG").grid(row=1, column=2)
        tk.Label(text="BEEP-CORNER").grid(row=1, column=3)

        self.corner = tk.Label()
        self.corner.grid(row=2, column=0)

        self.facing = tk.Label()
        self.facing.grid(row=2, column=1)

        self.beep_bag = tk.Label()
        self.beep_bag.grid(row=2, column=2)

        self.beep_corner = tk.Label()
        self.beep_corner.grid(row=2, column=3)

        self.canvas = tk.Canvas(
            width=((karel.world.width + 1) // 2 + 2) * self.size,
            height=((karel.world.height + 1) // 2 + 2) * self.size,
            background="black",
        )
        self.canvas.grid(column=0, row=3, columnspan=4)

        self.font = f"{font} {int(12 * scale)}"

        self.__draw_world(karel.world)
        self.render(karel)

    def __draw_world(self, world: "World"):
        m, n = (world.height + 1) // 2, (world.width + 1) // 2

        # NUMBERS
        self.canvas.create_text(
            self.size // 2,
            self.size,
            text="ST.",
            fill="white",
            anchor="s",
            font=self.font,
        )
        for i in range(m):
            self.canvas.create_text(
                self.size - 8,
                (i + 2) * self.size - self.size // 2,
                text=(m - i),
                anchor="e",
                fill="white",
                font=self.font,
            )
        y = ((m + 1) * self.size) + 8
        self.canvas.create_text(
            (n + 1) * self.size + self.size // 2,
            y,
            text="AVE.",
            fill="white",
            anchor="n",
            font=self.font,
        )
        for i in range(n):
            self.canvas.create_text(
                (i + 2) * self.size - self.size // 2,
                y,
                text=(i + 1),
                anchor="n",
                fill="white",
                font=self.font,
            )

        # BORDER
        self.canvas.create_rectangle(
            self.size,
            self.size,
            self.size * (n + 1),
            self.size * (m + 1),
            fill="",
            outline="white",
        )

        for row, col in product(range(world.height), range(world.width)):
            block = world.data[row][col]
            if row % 2 == 0 and col % 2 == 0:
                self.__print_beeper(world, block or ".", row // 2, col // 2)
            elif block == Block.Wall:
                size = self.size
                x = size + col // 2 * size
                y = (world.width + 1) // 2 * size - row // 2 * size
                if row % 2 == 1 and col % 2 == 0:
                    self.canvas.create_line(x, y, x + size, y, fill="white")
                elif row % 2 == 0:
                    x += size
                    self.canvas.create_line(x, y, x, y + size, fill="white")

        self.canvas.update()

    def __print_beeper(
        self, world: "World", beeper: Union[str, int], row: int, column: int
    ):
        self.canvas.create_text(
            (column + 1) * self.size + self.size // 2,
            ((world.height + 1) // 2 - row) * self.size + self.size // 2,
            text=str(beeper),
            anchor="center",
            fill="white",
            font=self.font,
        )

    def render(self, karel: "Karel"):
        self.facing["text"] = karel.direction.as_string

        self.steps["text"] = karel.steps
        self.last_cmd["text"] = karel.last_command
        self.corner["text"] = f"({(karel.x + 2) // 2}, {(karel.y + 2) // 2})"
        self.beep_bag["text"] = karel.beepers
        self.beep_corner["text"] = karel.world.data[karel.y][karel.x]

        i, j = (karel.y + 2) // 2, (karel.x + 2) // 2
        size = self.size
        x, y = j * size, ((karel.world.height + 1) // 2 - i + 1) * size
        self.canvas.create_rectangle(
            x + 1, y + 1, x + size - 1, y + size - 1, fill="black"
        )

        karel_on_map = karel.direction.in_world
        self.canvas.create_text(
            x + size // 2,
            y + size // 2,
            text=karel_on_map,
            font=self.font + " bold",
            fill="yellow",
            anchor="center",
        )

        self.canvas.update()
        self.canvas.after(karel.step_delay)

    def update(self, karel: "Karel", dx: int, dy: int):
        block = karel.world.data[karel.y - 2 * dy][karel.x - 2 * dx]

        i, j = (karel.y - 2 * dy) // 2, (karel.x - 2 * dx) // 2
        x, y = (j + 1) * self.size, (
            (karel.world.height + 1) // 2 - i
        ) * self.size

        self.canvas.create_rectangle(
            x + 1, y + 1, x + self.size - 1, y + self.size - 1, fill="black"
        )
        self.__print_beeper(karel.world, block or ".", i, j)

    def error(self, karel: "Karel", message: str):
        self.render(karel)

        self.last_cmd["foreground"] = "red"
        self.last_cmd["text"] = f"Error Shutoff! ({message})"


class World:
    def __init__(self, width, height, data):
        self.width = width * 2 - 1
        self.height = height * 2 - 1
        self.data = [
            [0 for _ in range(self.width)] for _ in range(self.height)
        ]

        if self.width > 30 or self.height > 30:
            raise ValueError(
                "The given world is greater than the max values of [{}x{}]".format(
                    30, 30
                )
            )

        self._parse_world(data)

    def __parse_wall(self, column, row, orientation, line):
        column = column * 2 - 2
        row = row * 2 - 2

        if column % 2 == 1 or row % 2 == 1:
            raise ValueError("Wrong position of the wall")

        if orientation == "E":
            column += 1
        elif orientation == "W":
            column -= 1
        elif orientation == "N":
            row += 1
        elif orientation == "S":
            row -= 1
        else:
            raise ValueError(
                "Unknown wall orientation '{}' on line {} in world file".format(
                    orientation, line
                )
            )

        self.data[row][column] = Block.Wall
        if column % 2 == 1 and row % 2 == 0:
            if row + 1 < self.height:
                self.data[row + 1][column] = Block.Wall
            if row - 1 >= 0:
                self.data[row - 1][column] = Block.Wall
        else:
            if column + 1 < self.width:
                self.data[row][column + 1] = Block.Wall
            if column - 1 >= 0:
                self.data[row][column - 1] = Block.Wall

    def __parse_beepers(self, column, row, count):
        column = column * 2 - 2
        row = row * 2 - 2
        self.data[row][column] = count

    def _parse_world(self, world_content):
        for i, line in enumerate(world_content, 1):
            if not line:
                continue

            block, column, row, arg = [part.upper() for part in line.split()]
            if block == "W":
                self.__parse_wall(int(column), int(row), arg, i)
            elif block == "B":
                self.__parse_beepers(int(column), int(row), int(arg))
            else:
                raise ValueError(
                    "Unknown block character '{}' on line {} in world file".format(
                        block, i
                    )
                )


class Karel:
    def __init__(
        self, filename, summary_mode=False, scale=1, font="monospace"
    ):
        """
        Turn Karel on.

        Args:
            filename: Path to the file containing Karel's world and initial
                settings.
            summary_mode: Disable rendering of the Karel's world in the window.

                Defaults to `False`.
            scale: Change the scaling factor of the font used to render Karel's
                world.

                Defaults to `1`.
            font: Change the default monospace font that is used to render the
                world.

                Defaults to `"monospace"`.
        """
        self.summary_mode = summary_mode

        world_content = None
        with open(filename) as world_file:
            world_content = world_file.read()

        first_line, world = world_content.split("\n", maxsplit=1)
        width, height, x, y, direction, beepers = first_line.split()
        direction = direction.upper()

        self.world = World(int(width), int(height), world.split("\n"))
        self.x, self.y = int(x) * 2 - 2, int(y) * 2 - 2

        if direction == "S":
            self.direction = Direction.South.value
        elif direction == "W":
            self.direction = Direction.West.value
        elif direction == "E":
            self.direction = Direction.East.value
        elif direction == "N":
            self.direction = Direction.North.value
        else:
            raise ValueError("Unknown Karel's direction")

        self.beepers = int(beepers)

        self.steps = 0
        self.step_delay = 1000
        self.is_running = True
        self.last_command = "turn_on"

        self._initialize_window(scale, font)

    @skip_for_summary
    def _initialize_window(self, scale, font):
        """
        Initializes a window where the Karel is drawn.
        """
        self._window = KarelWindow(self, scale, font)

    @skip_for_summary
    def __update(self, dx, dy):
        """
        Updates Karel's position in the window.

        Args:
            dx: Change along the horizontal axis.
            dy: Change along the vertical axis.
        """
        self._window.update(self, dx, dy)

    def __render(self, fn, *args, **kwargs):
        """
        Updates step counter and last command for Karel.
        If the robot is not running in a summary mode, then the window
        is rendered once again.
        """
        if not self.is_running:
            return

        self.steps += 1
        self.last_command = fn.__name__

        if not self.summary_mode:
            self._window.render(self)

    def __error(self, message):
        """
        Provides error handling.

        Args:
            message: Error message to be shown.
        """
        if not self.summary_mode:
            self._window.error(self, message)
            self.is_running = False
        else:
            print(f"Error Shutoff! ({message})", file=stderr)

    @skip_for_summary
    def __deinit(self):
        tk.mainloop()

    def __check_state(self, *args, **kwargs):
        """
        Assures that commands are not run while the robot is not running.

        Robot is not running when error has occurred.
        """
        return self.is_running

    @check_before(__check_state)
    def beepers_in_bag(self):
        """
        Check if there are any beepers in the Karel's bag.

        Returns:
            `True` if there is at least one beeper in Karel's beeper bag,
            `False` otherwise.
        """
        return self.beepers > 0

    def no_beepers_in_bag(self):
        """
        Check if there are no beepers in the Karel's bag.

        Returns:
            `True` if there is no beeper in Karel's beeper bag, `False` otherwise.
        """
        return not self.beepers_in_bag()

    @check_before(__check_state)
    def front_is_clear(self):
        """
        Check if there is no wall in front of Karel.

        Returns:
            `True` if there is no obstacle in front of Karel, `False` otherwise.
        """
        dx, dy = self.direction.vector
        if not (
            0 <= self.x + dx < self.world.width
            and 0 <= self.y + dy < self.world.height
        ):
            return False

        if self.world.data[self.y + dy][self.x + dx] == Block.Wall:
            return False

        return True

    def front_is_blocked(self):
        """
        Check if there is a wall in front of Karel.

        Returns:
            `True` if there is an obstacle in front of Karel, `False` otherwise.
        """
        return not self.front_is_clear()

    @check_before(__check_state)
    def __side_is_clear(self, d):
        """
        Helper function for determining whether the sides of Karel are clear.

        Args:
            d: Counterclockwise change in angle of Karel's heading.

        Returns:
            `True` if the side after applying the `d` counterclokwise turn
            is clear, `False` otherwise.
        """
        original_direction = self.direction
        self.direction = self.direction + d

        is_clear = self.front_is_clear()
        self.direction = original_direction

        return is_clear

    def left_is_clear(self):
        """
        Check if there is no wall to the left side of Karel.

        Returns:
            `True` if there is no obstacle to the left side of Karel,
            `False` otherwise.
        """
        return self.__side_is_clear(90)

    def left_is_blocked(self):
        """
        Check if there is a wall to the left side of Karel.

        Returns:
            `True` if there is an obstacle to the left side of Karel,
            `False` otherwise.
        """
        return not self.left_is_clear()

    def right_is_clear(self):
        """
        Check if there is no wall to the right side of Karel.

        Returns:
            `True` if there is no obstacle to the right side of Karel,
            `False` otherwise.
        """
        return self.__side_is_clear(-90)

    def right_is_blocked(self):
        """
        Check if there is a wall to the right side of Karel.

        Returns:
            `True` if there is an obstacle to the right side of Karel,
            `False` otherwise.
        """
        return not self.right_is_clear()

    @check_before(__check_state)
    def facing_north(self):
        """
        Check if Karel is facing north.

        Returns:
            `True` if Karel is facing north, `False` otherwise.
        """
        return self.direction == Direction.North

    def not_facing_north(self):
        """
        Check if Karel is »not« facing north.

        Returns:
            `True` if Karel is »not« facing north, `False` otherwise.
        """
        return not self.facing_north()

    @check_before(__check_state)
    def facing_south(self):
        """
        Check if Karel is facing south.

        Returns:
            `True` if Karel is facing south, `False` otherwise.
        """
        return self.direction == Direction.South

    def not_facing_south(self):
        """
        Check if Karel is »not« facing south.

        Returns:
            `True` if Karel is »not« facing south, `False` otherwise.
        """
        return not self.facing_south()

    @check_before(__check_state)
    def facing_east(self):
        """
        Check if Karel is facing east.

        Returns:
            `True` if Karel is facing east, `False` otherwise.
        """
        return self.direction == Direction.East

    def not_facing_east(self):
        """
        Check if Karel is »not« facing east.

        Returns:
            `True` if Karel is »not« facing east, `False` otherwise.
        """
        return not self.facing_east()

    @check_before(__check_state)
    def facing_west(self):
        """
        Check if Karel is facing west.

        Returns:
            `True` if Karel is facing west, `False` otherwise.
        """
        return self.direction == Direction.West

    def not_facing_west(self):
        """
        Check if Karel is »not« facing west.

        Returns:
            `True` if Karel is »not« facing west, `False` otherwise.
        """
        return not self.facing_west()

    @check_before(__check_state)
    def beepers_present(self):
        """
        Check whether Karel stands on a position that has any beepers present.

        Returns:
            `True` if there is at least one beeper present on the current
            position, `False` otherwise.
        """
        return self.world.data[self.y][self.x] > 0

    def no_beepers_present(self):
        """
        Check whether Karel stands on a position that has no beepers present.

        Returns:
            `True` if there is no beeper present on the current position,
            `False` otherwise.
        """
        return not self.beepers_present()

    @check_before(__check_state)
    @after(__render)
    def step(self):
        """
        Move Karel one position in the direction he's facing.
        """
        if not self.front_is_clear():
            return self.__error("Can't move this way")

        dx, dy = self.direction.vector
        self.x += 2 * dx
        self.y += 2 * dy
        self.__update(dx, dy)

    @check_before(__check_state)
    @after(__render)
    def turn_left(self):
        """
        Turn Karel counterclockwise, i.e. left, by 90 degrees.
        """
        self.direction = self.direction + 90

    @after(__render)
    def turn_off(self):
        """
        Turn Karel off.
        """
        self.is_running = False
        self.__deinit()

    @check_before(__check_state)
    @after(__render)
    def put_beeper(self):
        """
        Take a beeper from Karel's beeper bag and put on the current position.
        """
        if self.beepers <= 0:
            return self.__error("Karel has no beeper to put at the corner")

        self.world.data[self.y][self.x] += 1
        self.beepers -= 1

    @check_before(__check_state)
    @after(__render)
    def pick_beeper(self):
        """
        Pick a beeper from the current position and put it into Karel's
        beeper bag.
        """
        if self.world.data[self.y][self.x] <= 0:
            return self.__error("There is no beeper at the corner")

        self.world.data[self.y][self.x] -= 1
        self.beepers += 1

    def set_step_delay(self, delay):
        """
        Set delay of a Karel's step.

        Args:
            delay: Delay of one step in milliseconds.
        """
        self.step_delay = delay

    def get_step_delay(self):
        """
        Get current delay of a Karel's step.

        Returns:
            Current delay in milliseconds.
        """
        return self.step_delay
