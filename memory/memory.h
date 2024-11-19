//
// Created by sascha-roggatz on 25.10.24.
//

#ifndef CLOXVM_MEMORY_H
#define CLOXVM_MEMORY_H

#include "../common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity * 2))

#define GROW_ARRAY(type, pointer, oldCap, newCap) \
    (type*)reallocate(pointer, sizeof(type) * oldCap, sizeof(type) * newCap)

#define FREE_ARRAY(type, pointer, capacity) \
    reallocate(pointer, sizeof(type) * capacity, 0)


void *reallocate(void *pointer, size_t oldSize, size_t newSize);


#endif //CLOXVM_MEMORY_H
