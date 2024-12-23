//
// Created by sascha-roggatz on 25.10.24.
//

#ifndef CLOXVM_DEBUG_H
#define CLOXVM_DEBUG_H

#include "../chunk/chunk.h"

void disassembleChunk(Chunk *chunk, const char *name);

int disassembleInstruction(Chunk *chunk, int offset);

void printValue(Value value);

#endif //CLOXVM_DEBUG_H
