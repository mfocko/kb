#include "maze.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Checks if pointer points inside the memory specified by upper and lower
 * bound.
 * @param first Start of the allocated memory.
 * @param last First address that is not included in the allocated memory.
 * @param pos Pointer to memory.
 * @returns <code>true</code> if pos points to the memory [first, last), <code>
 * false</code> otherwise.
 */
static bool within_bounds(const char *first, const char *last, const char *pos)
{
    /* TODO */
}

/**
 * @brief Sets 2D coordinates of the pointer.
 * @param map Start of the 2D memory.
 * @param position Pointer to the map.
 * @param width Width of one row of the map.
 * @param row Output variable where the row is set.
 * @param col Output variable where the column is set.
 */
static void set_coordinates(const char *map, char *position, size_t width, size_t *row, size_t *col)
{
    /* TODO */
}

/**
 * @brief Checks if indices are within bounds of the 2D map.
 * @param width Width of the map.
 * @param height Height of the map.
 * @param row Row to be checked.
 * @param col Column to be checked.
 * @returns <code>true</code> if row,col are within bounds of the map,.<code>false
 * </code> otherwise.
 */
static bool within_bounds_by_index(size_t width, size_t height, size_t row, size_t col)
{
    /* TODO */
}

void print_maze(const char *map, char *position, char direction, size_t width, size_t height)
{
    printf("Maze:\n");
    for (size_t row = 0; row < height; row++) {
        for (size_t col = 0; col < width; col++) {
            const char *current = &map[row * width + col];

            if (current == position) {
                switch (direction) {
                case '^':
                    putchar('N');
                    break;
                case 'v':
                    putchar('S');
                    break;
                case '>':
                    putchar('E');
                    break;
                case '<':
                    putchar('W');
                    break;
                }
                continue;
            }

            putchar(*current);
        }
        putchar('\n');
    }
    putchar('\n');
}

enum end_state_t walk(const char *map, char *position, char direction, size_t width, size_t height)
{
    /* TODO */
    return NONE;
}
