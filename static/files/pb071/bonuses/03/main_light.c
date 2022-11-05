#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* If you want to see arrays before and after sorting, uncomment next line */
// #define DEBUG_OUTPUT

/**
 * @brief Swaps two elements at given addresses.
 * @param left Pointer to the first element.
 * @param right Pointer to the second element.
 */
void swap(int *left, int *right)
{
    /* TODO */
}

/**
 * @brief Get index of biggest element in the array.
 * @param ptr Pointer to the first element of the array.
 * @param count Count of the elements in the array.
 * @returns Index of the biggest element given the ordering.
 */
size_t maximum(int *ptr, size_t count)
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
 */
void select_sort(int *ptr, size_t count)
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

// #pragma region TESTS
/**
 * @brief Check if array is sorted.
 * @param ptr Pointer to the first element of the array.
 * @param count Count of the elements in the array.
 */
static void check_if_sorted(int *ptr, size_t count)
{
    int *left = ptr;
    int *right = ptr + 1;

    for (size_t i = 0; i < count - 1; i++) {
        assert(*left <= *right);

        left++;
        right++;
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

        select_sort(examples[i], count);

#ifdef DEBUG_OUTPUT
        printf("After sorting:");
        print_numbers(examples[i], count);
        printf("\n\n");
#endif

        check_if_sorted(examples[i], count);
    }
    printf("[PASS] Tests passed.\n\n");
}
// #pragma endregion TESTS

int main(void)
{
    /* Neither of those tests check for bad behaviour, like overwriting array
     * with elements satisfying ordering. **I** check those :scheming:
     */
    check_int_arrays();

    return 0;
}
