#ifndef CLOXVM_COMPILER_H
#define CLOXVM_COMPILER_H

#include "chunk.h"
#include "common.h"

bool compile(const char *source, Chunk *chunk);

#endif