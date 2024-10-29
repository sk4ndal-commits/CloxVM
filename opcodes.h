//
// Created by sascha-roggatz on 26.10.24.
//

#ifndef CLOXVM_OPCODES_H
#define CLOXVM_OPCODES_H

typedef enum {
    OP_RETURN,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_CONSTANT
} OpCode;

#endif //CLOXVM_OPCODES_H
