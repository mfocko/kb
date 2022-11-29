from karel_tk import Karel

import sys


class SuperKarel(Karel):
    # you can define your own helper functions on Karel here, if you wish to

    def run(self):
        # TODO
        pass


def main():
    if len(sys.argv) != 2:
        print("Please provide a path to the world as an argument.")
        return

    k = SuperKarel(sys.argv[1], scale=3, font="Iosevka\\ Term")
    k.set_step_delay(500)

    k.run()

    k.turn_off()


if __name__ == "__main__":
    main()
