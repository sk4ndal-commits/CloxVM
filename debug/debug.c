//
// Created by sascha-roggatz on 25.10.24.
//

#include <stdio.h>
#include "debug.h"
#include "../enums/opcodes.h"

int simpleInstruction(const char *name, int offset);

int constantInstruction(const char *name, Chunk *chunk, int offset);


void disassembleChunk(Chunk *chunk, const char *name) {

    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}


int disassembleInstruction(Chunk *chunk, const int offset) {
    printf("%04d ", offset);

    if (offset > 0 && chunk->lines[offset - 1] == chunk->lines[offset])
        printf("    | ");
    else
        printf("%4d ", chunk->lines[offset]);

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown instruction %d\n", instruction);
            return offset + 1;
    }
}

int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int negateInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

int constantInstruction(const char *name, Chunk *chunk, int offset) {
    uint8_t constantIdx = chunk->code[offset + 1];
    printf("%-16s %4d", name, constantIdx);
    printValue(chunk->constants.values[constantIdx]);
    printf("\n");

    return offset + 2;
}

void printValue(Value value) {
    printf("%g", value);
}

