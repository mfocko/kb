#include "maze.h"

#define CUT_MAIN
#include "cut.h"

static void check_result(char *map, size_t position, char direction, size_t width, size_t height, enum end_state_t expected)
{
    enum end_state_t state = walk(map, &map[position], direction, width, height);
    ASSERT(state == expected);
}

TEST(basic)
{
    SUBTEST(spawns_on_treasure)
    {
        check_result(("T."
                      ".."),
                0,
                '>',
                2,
                2,
                FOUND_TREASURE);
    }
    SUBTEST(spawns_on_key)
    {
        check_result(("K."
                      ".."),
                0,
                '^',
                2,
                2,
                FOUND_KEY);
    }
    SUBTEST(spawns_on_treasure_in_middle)
    {
        check_result(("..."
                      ".T."
                      "..."),
                4,
                'v',
                3,
                3,
                FOUND_TREASURE);
    }
}

TEST(some_walking)
{
    SUBTEST(straight_up)
    {
        check_result((".T."
                      "..."
                      "..."),
                7,
                '^',
                3,
                3,
                FOUND_TREASURE);
    }
    SUBTEST(down)
    {
        check_result(("..........................T"), 7, 'v', 1, 27, FOUND_TREASURE);
    }
    SUBTEST(to_right)
    {
        check_result(("..........................K"), 2, '>', 27, 1, FOUND_KEY);
    }
    SUBTEST(to_left)
    {
        check_result(("K.........................."), 15, '<', 27, 1, FOUND_KEY);
    }
}

TEST(follows_directions)
{
    SUBTEST(basic)
    {
        check_result((">..v"
                      "...."
                      "...K"
                      "^..<"),
                12,
                '>',
                4,
                4,
                FOUND_KEY);
    }
    SUBTEST(no_way_to_avoid)
    {
        const char *directions = "<>^v";

        for (size_t row = 0; row < 5; row++) {
            for (size_t col = 0; col < 5; col++) {
                for (size_t dir = 0; dir < 4; dir++) {
                    check_result((">>>>v"
                                  "^>>vv"
                                  "^^T<v"
                                  "^^<<<"
                                  "^<<<<"),
                            row * 5 + col,
                            directions[dir],
                            5,
                            5,
                            FOUND_TREASURE);
                }
            }
        }
    }
}

TEST(follows_directions_even_to_doom)
{
    SUBTEST(to_right)
    {
        check_result((".>.."
                      "^KTK"
                      ".TvT"
                      "...<"),
                1,
                '>',
                4,
                4,
                OUT_OF_BOUNDS);
    }
    SUBTEST(to_up)
    {
        check_result((".>.."
                      "^KTK"
                      ".TvT"
                      "...<"),
                4,
                '>',
                4,
                4,
                OUT_OF_BOUNDS);
    }
    SUBTEST(to_down)
    {
        check_result((".>.."
                      "^KTK"
                      ".TvT"
                      "...<"),
                10,
                '>',
                4,
                4,
                OUT_OF_BOUNDS);
    }
    SUBTEST(to_left)
    {
        check_result((".>.."
                      "^KTK"
                      ".TvT"
                      "...<"),
                15,
                '>',
                4,
                4,
                OUT_OF_BOUNDS);
    }
}

TEST(mean)
{
    SUBTEST(out_of_bounds_at_the_beginning)
    {
        check_result((".."
                      ".."),
                4,
                'v',
                2,
                2,
                OUT_OF_BOUNDS);
        check_result((".."
                      ".."),
                5,
                'v',
                2,
                2,
                OUT_OF_BOUNDS);
        check_result((".."
                      ".."),
                (size_t) -1,
                'v',
                2,
                2,
                OUT_OF_BOUNDS);
        check_result((">T<"
                      "..."
                      "^.^"),
                10,
                '^',
                3,
                3,
                OUT_OF_BOUNDS);
    }
}

TEST(infinite_bonus)
{
    SUBTEST(easy)
    {
        check_result((">.v"
                      "..."
                      "^.<"),
                0,
                'v',
                3,
                3,
                INFINITE_LOOP);
    }
    SUBTEST(medium)
    {
        check_result((">v"
                      "^<"),
                2,
                'v',
                2,
                2,
                INFINITE_LOOP);
    }
    SUBTEST(hard)
    {
        check_result(("v.v"
                      "..."
                      "^.^"),
                0,
                '>',
                3,
                3,
                INFINITE_LOOP);
    }
    SUBTEST(harder)
    {
        check_result((">.v.."
                      "....."
                      "^...<"
                      "....."
                      "..>.^"),
                4,
                'v',
                5,
                5,
                INFINITE_LOOP);
    }
}
