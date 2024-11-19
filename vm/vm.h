//
// Created by sascha-roggatz on 28.10.24.
//

#ifndef CLOXVM_VM_H
#define CLOXVM_VM_H

#include "../common.h"
#include "../chunk/chunk.h"
#include "../enums/interpretresult.h"
#include "../value/value.h"

#define STACK_MAX 256

typedef struct {
    Chunk *chunk;
    uint8_t *ip;
    Value stack[STACK_MAX];
    Value *stackTop;
} VM;

void initVM();

void freeVM();

InterpretResult interpret(const char *source);

static InterpretResult run();

void push(Value value);

Value pop();

#endif //CLOXVM_VM_H
