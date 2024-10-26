#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "opcodes.h"

int main(void) {

    Chunk chunk;

    initChunk(&chunk);
    int constant = addConstant(&chunk, 1.2);

    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constant, 123);

    disassembleChunk(&chunk, "test_chunk");


    freeChunk(&chunk);


    return 0;
}
