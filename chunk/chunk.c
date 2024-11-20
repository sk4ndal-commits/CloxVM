//
// Created by sascha-roggatz on 25.10.24.
//

#include "../chunk/chunk.h"
#include "../memory/memory.h"

#include <stdint.h>


/**
 * Initializes a Chunk struct.
 *
 * This function prepares a Chunk struct for use by setting its initial
 * count and capacity to zero and its code and lines pointers to NULL.
 * It also initializes the constants ValueArray of the Chunk.
 *
 * @param chunk A pointer to the Chunk struct to be initialized.
 */
void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}


/**
 * Writes a byte and its corresponding line number into the chunk.
 *
 * This function checks if the current capacity of the chunk is sufficient
 * to accommodate the new byte. If the capacity is insufficient, it grows
 * the capacity of the chunk using the GROW_CAPACITY and GROW_ARRAY macros.
 * It then writes the byte and line number at the current count index
 * and increments the count.
 *
 * @param chunk A pointer to the Chunk struct where the byte and line number will be written.
 * @param byte The byte to be written into the chunk.
 * @param line The line number associated with the byte.
 */
void writeChunk(Chunk *chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t,
                                 chunk->code,
                                 oldCapacity,
                                 chunk->capacity);
        chunk->lines = GROW_ARRAY(int,
                                  chunk->lines,
                                  oldCapacity,
                                  chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

/**
 * Frees the memory allocated for the components of the given chunk.
 *
 * This function deallocates the memory associated with the code and lines arrays
 * of the chunk, as well as the values in the chunk's constants array. After
 * cleaning up, it reinitializes the chunk to its default state.
 *
 * @param chunk A pointer to the Chunk struct whose memory will be freed.
 */
void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

/**
 * Adds a value to the constants array of the given chunk.
 *
 * @param chunk A pointer to the Chunk struct where the constant will be added.
 * @param value The Value to be added to the chunk's constants array.
 * @return The index of the added constant in the chunk's constants array.
 */
int addConstant(Chunk *chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}
