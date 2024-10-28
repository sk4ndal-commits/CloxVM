//
// Created by sascha-roggatz on 28.10.24.
//

#include <bzlib.h>
#include "vm.h"
#include "opcodes.h"
#include "debug.h"

static void resetStack();

VM vm;


void initVM() {
    resetStack();
}

void freeVM() {

}


InterpretResult interpret(Chunk *chunk) {
    vm.chunk = chunk;
    vm.ip = chunk->code;
    return run();
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

    for (;;) {
        uint8_t ins;

#ifdef DEBUG_TRACE_EXECUTION
        printf("stack: ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
#endif

        switch (ins = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NEGATE: {
                push(-pop());
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

void push(Value value) {

    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

static void resetStack() {
    vm.stackTop = vm.stack;
}

