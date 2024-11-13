cmake_minimum_required(VERSION 3.29)
project(cloxvm C)

set(CMAKE_C_STANDARD 17)

add_executable(cloxvm main.c
        common.h
        chunk.h
        chunk.c
        memory.h
        memory.c
        debug.c
        debug.h
        value.c
        value.h
        opcodes.h
        vm.c
        vm.h
        interpretresult.h
        compiler.c
        scanner.c
)
