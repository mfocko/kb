#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define UNUSED(x) ((void) (x))

/**
 * @brief Counts occurences of any substring in the file.
 * @param file File where the substring is to be counted in.
 * @param substring Substring to be counted.
 * @returns Count of occurences of the substring the given file.
 */
long count_anything(FILE *file, const char *substring)
{
    /* TODO */
    UNUSED(file);
    UNUSED(substring);
    return 0;
}

/**
 * @brief Counts ananas. Nothing more to it.
 * @param file File where the ananas is to be counted in.
 * @returns Count of occurences of ananas.
 */
long count_ananas(FILE *file)
{
    return count_anything(file, "ananas");
}

/**
 * @brief Writes given number to the file, character by character.
 * @param file File where the number is supposed to be written.
 * @param number Number to be written.
 */
void write_number(FILE *file, long number)
{
    /* TODO */
    UNUSED(file);
    UNUSED(number);
}

/**
 * @brief Main function of a program.
 * @returns Exit code that denotes following:
 *      0 in case of success
 *      1 in case of invalid usage
 *      2 in case of failure on input file
 *      3 in case of failure on output file
 */
int main(int argc, char **argv)
{
    if (argc < 3 || argc > 4) {
        printf("Usage: %s <input-file> <output-file> [string-to-be-counted]\n", argv[0]);
        return 1;
    }

    /* TODO */
    return 0;
}
