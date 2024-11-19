cmake_minimum_required(VERSION 3.29)
project(cloxvm C)

set(CMAKE_C_STANDARD 17)

add_executable(cloxvm main.c
        common.h
        chunk/chunk.h
        chunk/chunk.c
        memory/memory.h
        memory/memory.c
        debug/debug.c
        debug/debug.h
        value/value.c
        value/value.h
        enums/opcodes.h
        vm/vm.c
        vm/vm.h
        enums/interpretresult.h
        compiler/compiler.c
        scanner/scanner.c
)
