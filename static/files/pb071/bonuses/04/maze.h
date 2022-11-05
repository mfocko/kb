#include <stdlib.h>

enum end_state_t
{
    NONE,
    FOUND_KEY,
    FOUND_TREASURE,
    OUT_OF_BOUNDS,
    INFINITE_LOOP,
};

/**
 * @brief Prints maze and the robot within it.
 * @param map Map of the maze.
 * @param position Current position of the robot.
 * @param direction Direction the robot is facing, one of "^v<>".
 * @param width Width of the map.
 * @param height Height of the map.
 */
void print_maze(const char *map, char *position, char direction, size_t width, size_t height);

/**
 * @brief Get end state of the robot after his walk.
 * @param map Map of the maze.
 * @param position Initial position of the robot in the maze.
 * @param direction Direction the robot is facing at the beginning. You can assume
 * correctness of this value.
 * @param width Width of the maze. You can assume correctness of this value.
 * @param height Height of the maze. You can assume correctness of this value.
 * @returns End state of the robot after his walk is finished or has been terminated
 * manually.
 */
enum end_state_t walk(const char *map, char *position, char direction, size_t width, size_t height);
