//
// Created by sascha-roggatz on 28.10.24.
//

#ifndef CLOXVM_INTERPRETRESULT_H
#define CLOXVM_INTERPRETRESULT_H

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILER_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

#endif //CLOXVM_INTERPRETRESULT_H
