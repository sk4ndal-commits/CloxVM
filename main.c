
#include "chunk.h"
#include "opcodes.h"
#include "vm.h"

int main(void) {

    initVM();

    Chunk chunk;


    initChunk(&chunk);
    int four = addConstant(&chunk, 4.0);
    int five = addConstant(&chunk, 5.0);
    int zero = addConstant(&chunk, 0.0);

    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, four, 123);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, zero, 123);
    writeChunk(&chunk, OP_MULTIPLY, 123);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, five, 123);
    writeChunk(&chunk, OP_DIVIDE, 123);
    writeChunk(&chunk, OP_RETURN, 123);


    interpret(&chunk);

    freeVM();
    freeChunk(&chunk);


    return 0;
}
