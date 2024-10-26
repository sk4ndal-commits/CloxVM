//
// Created by sascha-roggatz on 26.10.24.
//

#ifndef CLOXVM_VALUE_H
#define CLOXVM_VALUE_H

#include "common.h"

typedef double Value;

typedef struct {
    int count;
    int capacity;
    Value *values;
} ValueArray;


void initValueArray(ValueArray *valueArray);

void writeValueArray(ValueArray *valueArray, Value value);

void freeValueArray(ValueArray *valueArray);

#endif //CLOXVM_VALUE_H
