//
// Created by sascha-roggatz on 25.10.24.
//

#include <stdio.h>
#include "debug.h"
#include "opcodes.h"

int simpleInstruction(const char* name, int offset);
int constantInstruction(const char *name, Chunk* chunk, int offset);
void printValue(Value value);

void disassembleChunk(Chunk* chunk, const char* name) {

    printf("== %s ==\n", name);

    for (int offset=0; offset<chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}


int disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown instruction %d\n", instruction);
            return offset+1;
    }
}

int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset+1;
}

int constantInstruction(const char* name, Chunk* chunk, int offset) {
    printf("%s\n", name);
    uint8_t constantIdx = chunk->code[offset +  1];
    printf("%-16s %4d", name, constantIdx);
    printValue(chunk->constants.values[constantIdx]);
    printf("\n");

    return offset + 2;
}

void printValue(Value value) {
    printf("%g", value);
}

