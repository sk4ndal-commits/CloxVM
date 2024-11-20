//
// Created by sascha-roggatz on 26.10.24.
//

#include "../value/value.h"
#include "../memory/memory.h"

/**
 * Initializes a ValueArray by resetting its count and capacity to zero and
 * setting its values pointer to NULL. This prepares the ValueArray for use,
 * ensuring it starts in a consistent state with no allocated memory.
 *
 * @param valueArray A pointer to the ValueArray to initialize.
 */
void initValueArray(ValueArray *valueArray) {
    valueArray->count = 0;
    valueArray->capacity = 0;
    valueArray->values = NULL;
}

/**
 * Appends a new value to the end of a ValueArray. If the array's capacity
 * is insufficient to hold the new value, the capacity is increased to
 * accommodate it.
 *
 * @param valueArray A pointer to the ValueArray to which the value should be added.
 * @param value The Value to add to the ValueArray.
 */
void writeValueArray(ValueArray *valueArray, Value value) {
    if (valueArray->capacity < valueArray->count + 1) {
        int oldCapacity = valueArray->capacity;
        valueArray->capacity = GROW_CAPACITY(oldCapacity);
        valueArray->values = GROW_ARRAY(Value, valueArray->values, oldCapacity,
                                        valueArray->capacity);
    }

    valueArray->values[valueArray->count] = value;
    valueArray->count++;
}

/**
 * Frees the memory allocated for the values in the ValueArray and reinitializes it.
 * This involves deallocating any memory that was allocated for the values,
 * and then resetting the ValueArray to an initialized state with no allocated memory.
 *
 * @param valueArray A pointer to the ValueArray whose memory is to be freed and reinitialized.
 */
void freeValueArray(ValueArray *valueArray) {
    FREE_ARRAY(Value, valueArray->values, valueArray->capacity);
    initValueArray(valueArray);
}
