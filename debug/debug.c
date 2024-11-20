//
// Created by sascha-roggatz on 25.10.24.
//

#include <stdio.h>
#include "debug.h"
#include "../enums/opcodes.h"

int simpleInstruction(const char *name, int offset);

int constantInstruction(const char *name, Chunk *chunk, int offset);


/**
 * Disassembles a given chunk of bytecode, printing a human-readable version.
 *
 * The function iterates over each instruction in the chunk, disassembling it
 * and printing the result to standard output.
 *
 * @param chunk The chunk of bytecode to disassemble.
 * @param name A name to associate with the chunk, printed as a header.
 */
void disassembleChunk(Chunk *chunk, const char *name) {

    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}


/**
 * Disassembles a single instruction in a bytecode chunk and prints its human-readable form.
 *
 * The function determines the type of instruction at the given offset within the chunk,
 * and prints the corresponding human-readable name and associated information. It adjusts
 * the current offset based on the length of the instruction.
 *
 * @param chunk The chunk of bytecode containing the instruction.
 * @param offset The offset in the chunk where the instruction begins.
 * @return The new offset after the disassembled instruction, accounting for the instruction's length.
 */
int disassembleInstruction(Chunk *chunk, const int offset) {
    printf("%04d ", offset);

    if (offset > 0 && chunk->lines[offset - 1] == chunk->lines[offset])
        printf("    | ");
    else
        printf("%4d ", chunk->lines[offset]);

    uint8_t instruction = chunk->code[offset];

    switch (instruction) {
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown instruction %d\n", instruction);
            return offset + 1;
    }
}

/**
 * Prints the name of an instruction and increments the offset.
 *
 * This function prints the provided instruction name to standard output
 * and then increments the offset by one. It is typically used for instructions
 * that do not require additional data.
 *
 * @param name The name of the instruction to print.
 * @param offset The current offset in the bytecode.
 * @return The updated offset, incremented by one.
 */
int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

/**
 * Outputs the name associated with a negate instruction and updates the byte offset.
 *
 * This function prints the provided name to standard output and increments the
 * provided byte offset by one.
 *
 * @param name The name associated with the instruction to be printed.
 * @param offset The current byte offset of the instruction.
 * @return The updated byte offset after processing the instruction.
 */
int negateInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

/**
 * Disassembles a constant instruction in the chunk of bytecode.
 *
 * Prints the instruction name, followed by the constant index and the constant's value.
 *
 * @param name The name of the instruction to be disassembled.
 * @param chunk The chunk of bytecode containing the instruction.
 * @param offset The current offset in the bytecode where the instruction starts.
 * @return The new offset in the bytecode after the instruction.
 */
int constantInstruction(const char *name, Chunk *chunk, int offset) {
    uint8_t constantIdx = chunk->code[offset + 1];
    printf("%-16s %4d", name, constantIdx);
    printValue(chunk->constants.values[constantIdx]);
    printf("\n");

    return offset + 2;
}

/**
 * Prints the given value to standard output in a formatted manner.
 *
 * This function uses the printf function to print a double value according to
 * the "%g" format specifier.
 *
 * @param value The value of type double to be printed.
 */
void printValue(Value value) {
    printf("%g", value);
}

