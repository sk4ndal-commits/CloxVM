//
// Created by sascha-roggatz on 25.10.24.
//

#include <stdlib.h>
#include "memory.h"

/**
 * Reallocates memory for a given pointer to a new size.
 *
 * @param pointer    The original memory block pointer.
 * @param oldSize    The size of the original memory block.
 * @param newSize    The size of the new memory block.
 * @return           A pointer to the newly allocated memory block, or NULL if newSize is 0.
 */
void* reallocate(void *pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}
