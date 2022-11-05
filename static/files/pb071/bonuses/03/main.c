#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* If you want to see arrays before and after sorting, uncomment next line */
// #define DEBUG_OUTPUT

/**
 * @brief Swaps two elements at given addresses of given size.
 * @param left Pointer to the first element.
 * @param right Pointer to the second element.
 * @param size Size of the memory one element takes.
 */
void swap(void *left, void *right, size_t size)
{
    /* TODO */
}

/**
 * @brief Get index of biggest element in the array.
 * @param ptr Pointer to the first element of the array.
 * @param count Count of the elements in the array.
 * @param size Size of one element in the array.
 * @param comp Comparator that is used to decide ordering of the elements.
 * @returns Index of the biggest element given the ordering.
 */
size_t maximum(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *))
{
    /* Pseudocode:
     * ===========
     * max_index <- 0
     * FOR i <- 1 TO n - 1 DO
     *   IF A[i] > A[max_index] THEN
     *     max_index <- i
     *   FI
     * OD
     * RETURN max_index
     */
    /* TODO */
    return 0;
}

/**
 * @brief Sort array in-situ using select-sort.
 * @param ptr Pointer to the first element of the array.
 * @param count Count of the elements in the array.
 * @param size Size of one element in the array.
 * @param comp Comparator that is used to decide ordering of the elements.
 */
void select_sort(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *))
{
    /* Pseudocode:
     * ===========
     * FOR i <- n - 1 DOWNTO 1 DO
     *   j <- MAXIMUM(A, i + 1)
     *   SWAP(A, i, j)
     * OD
     */
    /* TODO */
}

/**
 * @brief Compares two integers that are given through generic pointers.
 * @param x Pointer to the integer x.
 * @param y Pointer to the integer y.
 * @returns 0 if x == y, <0 if x < y, >0 otherwise.
 */
static int int_comparator(const void *x, const void *y)
{
    /* TODO */
    return 1;
}

/**
 * @brief Compares two characters by ASCII value.
 * @param x Pointer to the character x.
 * @param y Pointer to the character y.
 * @returns 0 if x == y, <0 if x < y, >0 otherwise.
 */
static int char_comparator(const void *x, const void *y)
{
    char x_value = *(const char *) x;
    char y_value = *(const char *) y;
    return x_value - y_value;
}

/**
 * @brief Compares two characters by ASCII value in a reversed order.
 * @param x Pointer to the character x.
 * @param y Pointer to the character y.
 * @returns 0 if x == y, >0 if x < y, <0 otherwise.
 */
static int char_reversed_comparator(const void *x, const void *y)
{
    char x_value = *(const char *) x;
    char y_value = *(const char *) y;
    return y_value - x_value;
}

// #pragma region TESTS
/**
 * @brief Check if array is sorted.
 * @param ptr Pointer to the first element of the array.
 * @param count Count of the elements in the array.
 * @param size Size of one element in the array.
 * @param comp Comparator that is used to decide ordering of the elements.
 */
static void check_if_sorted(void *ptr, size_t count, size_t size, int (*comp)(const void *, const void *))
{
    char *left = ptr;
    char *right = (char *) ptr + size;

    for (size_t i = 0; i < count - 1; i++) {
        assert(comp(left, right) <= 0);

        left += size;
        right += size;
    }
}

#ifdef DEBUG_OUTPUT
/**
 * @brief Print numbers from an array.
 * @param array Pointer to the first element of the array.
 * @param count Count of the elements in the array.
 */
static void print_numbers(int *array, size_t count)
{
    for (size_t i = 0; i < count; i++) {
        printf(" %d", array[i]);
    }
}
#endif

/**
 * @brief Run some basic tests on integer arrays.
 */
static void check_int_arrays()
{
    /* You are free to add any other examples you like, in case you want to add
     * array with more than 10 elements, please adjust the size.
     */

    size_t examples_count = 5;
    int examples[][10] = {
        { -1, 50, -10, 20, 30, 0, -100 },
        { -100, 0, 30, 20, -10, 50, -1 },
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        { -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    printf("[TEST] Integer arrays\n");
    for (size_t i = 0; i < examples_count; i++) {
        size_t count = sizeof(examples[i]) / sizeof(int);

#ifdef DEBUG_OUTPUT
        printf("Before sorting:");
        print_numbers(examples[i], count);
        putchar('\n');
#endif

        select_sort(examples[i], count, sizeof(int), int_comparator);

#ifdef DEBUG_OUTPUT
        printf("After sorting:");
        print_numbers(examples[i], count);
        printf("\n\n");
#endif

        check_if_sorted(examples[i], count, sizeof(int), int_comparator);
    }
    printf("[PASS] Tests passed.\n\n");
}

/**
 * @brief Run some basic tests on strings. Also with reversed ordering.
 */
static void check_char_arrays()
{
#define MAX_SIZE 20
    /* You are free to add any other examples you like, in case you want to add
     * strings with more than 10 characters, please adjust the size.
     * mind the terminator ;)
     */

    const size_t examples_count = 5;
    char examples[][MAX_SIZE] = {
        "Hello World!",
        "hi",
        "aloha",
        "LET US SORT",
        "LeT uS sOrT"
    };

    printf("[TEST] Char arrays\n");
    for (size_t i = 0; i < examples_count; i++) {
        size_t count = strlen(examples[i]);

        char temp_array[MAX_SIZE];
        strncpy(temp_array, examples[i], MAX_SIZE);
        select_sort(temp_array, count, 1, char_comparator);

#ifdef DEBUG_OUTPUT
        printf("Before sorting: \"%s\"\n", examples[i]);
        printf("After sorting: \"%s\"\n\n", temp_array);
#endif

        check_if_sorted(temp_array, count, 1, char_comparator);
    }
    printf("[PASS] Tests passed.\n\n");

    printf("[TEST] Char arrays reversed\n");
    for (size_t i = 0; i < examples_count; i++) {
        size_t count = strlen(examples[i]);

        char temp_array[MAX_SIZE];
        strncpy(temp_array, examples[i], MAX_SIZE);
        select_sort(temp_array, count, 1, char_reversed_comparator);

#ifdef DEBUG_OUTPUT
        printf("Before sorting: \"%s\"\n", examples[i]);
        printf("After sorting: \"%s\"\n\n", temp_array);
#endif

        check_if_sorted(temp_array, count, 1, char_reversed_comparator);
    }
    printf("[PASS] Tests passed.\n\n");
#undef MAX_SIZE
}

static void annotate(const char *description, bool result)
{
    printf("[TEST] %s\n", description);
    assert(result);
    printf("[PASS] Test passed.\n\n");
}

static void check_int_comparator()
{
    int a = 5, b = -10, c = -10;

    annotate("Test comparator for: a > b", int_comparator(&a, &b) > 0);
    annotate("Test comparator for: b < a", int_comparator(&b, &a) < 0);
    annotate("Test comparator for: b = c", int_comparator(&b, &c) == 0);
    annotate("Test comparator for: a = a", int_comparator(&a, &a) == 0);
    annotate("Test comparator for: b = b", int_comparator(&b, &b) == 0);
    annotate("Test comparator for: c = c", int_comparator(&c, &c) == 0);
}
// #pragma endregion TESTS

int main(void)
{
    /* Neither of those tests check for bad behaviour, like overwriting array
     * with elements satisfying ordering. **I** check those :scheming:
     */
    check_int_comparator();
    check_int_arrays();
    check_char_arrays();

    return 0;
}
