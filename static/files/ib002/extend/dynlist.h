#include <stdbool.h>
#include <stdlib.h>

struct dynamic_array_t
{
    char *data;
    size_t count;
    size_t capacity;
    size_t size;
};

/**
 * @brief Initializes dynamic array. Sets size of single element and zeroes
 * everything else.
 * @param arr Array to be initialized.
 * @param size Size of one element in the array.
 */
void dynamic_array_init(struct dynamic_array_t *arr, size_t size);

/**
 * @brief Destroys dynamic array. Deallocates all the memory and resets fields.
 * @param arr Array to be destroyed.
 */
void dynamic_array_destroy(struct dynamic_array_t *arr);

/**
 * @brief Returns pointer to the element on the given index.
 * @param arr Array containing the element.
 * @param index Index of the element.
 * @returns Pointer to the element, <code>NULL</code> if index is out of bounds.
 */
void *dynamic_array_at(struct dynamic_array_t *arr, size_t index);

/**
 * @brief Returns pointer to the first element of the array.
 * @param arr Array containing the element.
 * @returns Pointer to the first element, <code>NULL</code> if no elements are
 * present.
 */
void *dynamic_array_front(struct dynamic_array_t *arr);

/**
 * @brief Returns pointer to the last element of the array.
 * @param arr Array containing the element.
 * @returns Pointer to the last element, <code>NULL</code> if no elements are
 * present.
 */
void *dynamic_array_back(struct dynamic_array_t *arr);

/**
 * @brief Adds element to the end of the array.
 * @param arr Array where the element is to be added.
 * @param data Pointer to the data, that are to be copied into the array.
 * @returns <code>true</code> if element added successfully, <code>false</code>
 * otherwise.
 */
bool dynamic_array_push_back(struct dynamic_array_t *arr, void *data);

/**
 * @brief Removes last element from the array.
 * @param arr Array from which the last element is to be removed.
 */
void dynamic_array_pop_back(struct dynamic_array_t *arr);

/**
 * @brief Extends array with the elements from another array.
 * @param arr Array to be extended.
 * @param src Array from which the elements are copied.
 */
void dynamic_array_extend(struct dynamic_array_t *arr, struct dynamic_array_t *src);

/**
 * @brief Clears out the array.
 * @param arr Array to be cleared.
 */
void dynamic_array_clear(struct dynamic_array_t *arr);
