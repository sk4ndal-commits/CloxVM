//
// Created by sascha-roggatz on 25.10.24.
//

#ifndef CLOXVM_CHUNK_H
#define CLOXVM_CHUNK_H

#include "../common.h"
#include "../value/value.h"


typedef struct {
    int count;
    int capacity;
    uint8_t *code;
    int *lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);

#endif //CLOXVM_CHUNK_H
