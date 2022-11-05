#include "dynlist.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void dynamic_array_init(struct dynamic_array_t *arr, size_t size)
{
    if (arr == NULL)
    {
        return;
    }

    arr->data = NULL;
    arr->count = 0;
    arr->capacity = 0;
    arr->size = size;
}

void dynamic_array_destroy(struct dynamic_array_t *arr)
{
    if (arr == NULL)
    {
        return;
    }

    if (arr->data != NULL)
    {
        free(arr->data);
        arr->data = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

void *dynamic_array_at(struct dynamic_array_t *arr, size_t index)
{
    if (arr == NULL || index >= arr->count)
    {
        return NULL;
    }

    return arr->data + index * arr->size;
}

void *dynamic_array_front(struct dynamic_array_t *arr)
{
    if (arr == NULL || arr->count < 1)
    {
        return NULL;
    }

    return arr->data;
}

void *dynamic_array_back(struct dynamic_array_t *arr)
{
    if (arr == NULL || arr->count < 1)
    {
        return NULL;
    }

    return arr->data + (arr->count - 1) * arr->size;
}

/**
 * @brief Resize the dynamic array when needed.
 * @param arr Array to be resized.
 * @returns <code>true</code> if resizing was successful or array was not resized,
 * <code>false</code> otherwise.
 */
static bool dynamic_array_resize(struct dynamic_array_t *arr)
{
    if (arr == NULL || arr->count < arr->capacity)
    {
        return true;
    }

    size_t new_capacity = arr->capacity ? (arr->capacity * 2) : 16;
    void *new_data = realloc(arr->data, new_capacity * arr->size);
    if (new_data == NULL)
    {
        // failed to reallocate memory
        return false;
    }

    arr->data = new_data;
    arr->capacity = new_capacity;
    return true;
}

bool dynamic_array_push_back(struct dynamic_array_t *arr, void *data)
{
    if (arr == NULL || data == NULL)
    {
        return false;
    }

    if (!dynamic_array_resize(arr))
    {
        // failed to reallocate memory
        return false;
    }

    memcpy(dynamic_array_at(arr, arr->count), data, arr->size);
    arr->count++;

    return true;
}

void dynamic_array_pop_back(struct dynamic_array_t *arr)
{
    if (arr == NULL || arr->count < 1)
    {
        return;
    }

    arr->count--;
}

void dynamic_array_extend(struct dynamic_array_t *arr, struct dynamic_array_t *src)
{
    if (arr == NULL || src == NULL)
    {
        return;
    }

    for (size_t i = 0; i < src->count; i++)
    {
        dynamic_array_push_back(arr, dynamic_array_at(src, i));
    }
}

void dynamic_array_clear(struct dynamic_array_t *arr)
{
    if (arr == NULL)
    {
        return;
    }

    arr->count = 0;
}
