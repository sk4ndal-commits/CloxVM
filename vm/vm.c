//
// Created by sascha-roggatz on 28.10.24.
//

#include "vm.h"
#include "../enums/opcodes.h"
#include "../debug/debug.h"
#include "../compiler/compiler.h"
#include <stdio.h>

static void resetStack();

VM vm;


void initVM() {
    resetStack();
}

void freeVM() {

}


/**
 * Interprets the given source code.
 *
 * This function compiles the provided source code into a bytecode `Chunk`,
 * sets up the virtual machine (VM) to interpret that chunk, and then runs
 * the VM. If the compilation or execution fails, appropriate error results
 * will be returned.
 *
 * @param source The source code to interpret.
 * @return The result of the interpretation. It will be INTERPRET_OK if the
 *         interpretation completed successfully, INTERPRET_COMPILE_ERROR if
 *         there was a compilation error, or INTERPRET_RUNTIME_ERROR if there
 *         was a runtime error.
 */
InterpretResult interpret(const char *source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = chunk.code;

    InterpretResult result = run();

    freeChunk(&chunk);

    return INTERPRET_OK;
}

/**
 * Executes the bytecode in the virtual machine (VM).
 *
 * This function runs the main loop of the VM, reading bytecode instructions
 * one by one and executing them. The function handles various operations such
 * as addition, subtraction, multiplication, division, negation, pushing
 * constants, and returning results. It also optionally outputs debug
 * information about the stack and instructions being executed.
 *
 * @return The result of the interpretation. It will be INTERPRET_OK if the
 *         interpretation completed successfully.
 */
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)               \
    do {                            \
        Value a = pop();            \
        Value b = pop();            \
        Value result = a op b;      \
        push(result);               \
                                    \
    } while(false)

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
            case OP_ADD: {
                BINARY_OP(+);
                break;
            }
            case OP_SUBTRACT: {
                BINARY_OP(-);
                break;
            }
            case OP_MULTIPLY: {
                BINARY_OP(*);
                break;
            }
            case OP_DIVIDE: {
                BINARY_OP(/);
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
            default: break;
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

/**
 * Pushes a value onto the VM stack.
 *
 * This function takes a `Value` and places it on top of the stack, then
 * increments the stack pointer `stackTop` to point to the new top of the stack.
 *
 * @param value The value to be pushed onto the stack.
 */
void push(Value value) {

    *vm.stackTop = value;
    vm.stackTop++;
}

/**
 * Pops a value from the VM stack.
 *
 * This function decrements the stack pointer and returns the value that was at the top of the stack.
 *
 * @return The value that was at the top of the stack before decrementing the stack pointer.
 */
Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

/**
 * Resets the VM stack pointers.
 *
 * This function sets the `stackTop` pointer of the VM to the base of the stack.
 * It effectively clears the stack by resetting the top to the bottom of the stack array.
 * This function is typically called to initialize or reset the state of the virtual machine.
 */
static void resetStack() {
    vm.stackTop = vm.stack;
}

