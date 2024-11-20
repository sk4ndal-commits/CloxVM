#include <stdio.h>

#include "../compiler/compiler.h"

#include <stdlib.h>

#include "../scanner/scanner.h"
#include "../enums/opcodes.h"

typedef struct {
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;
Chunk *compilingChunk;


static Chunk* currentChunk();
static uint8_t makeConstant(Value value);
static void emitConstant(Value value);
static void emitByte(uint8_t byte);
static void emitBytes(uint8_t byte1, uint8_t byte2);
static void emitReturn();
static void endCompiler();
static void expression();
static void number();
static void advance();
static void consume(TokenType tokenType, const char *message);
static void errorAtCurrent(const char *message);
static void errorAtPrevious(const char *message);
static void errorAt(const Token *token, const char *message);

bool compile(const char *source, Chunk *chunk) {
    initScanner(source);
    compilingChunk = chunk;

    parser.panicMode = false;
    parser.hadError = false;

    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression");

    endCompiler();
    return !parser.hadError;
}

static void expression() {

}

static void number() {
    const double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

static uint8_t makeConstant(Value value) {
    int constant = addConstant()
}

static void endCompiler() {
    emitReturn();
}

static void emitReturn() {
    emitByte(OP_RETURN);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static Chunk* currentChunk() {
    return compilingChunk;
}

static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) continue;

        errorAtCurrent(parser.current.start);
    }
}

static void consume(const TokenType tokenType, const char *message) {
    if (parser.current.type == tokenType) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

static void errorAtPrevious(const char *message) {
    errorAt(&parser.previous, message);
}
static void errorAt(const Token *token, const char *message) {
    if (parser.panicMode) return;
    parser.panicMode = true;

    fprintf(stderr, "[line %d] Error", token->line);

    switch (token->type) {
        case TOKEN_EOF: {
            fprintf(stderr, " end of file");
        }
        break;
        case TOKEN_ERROR:
        break;
        default: {
            fprintf(stderr, " at '%.*s'", token->length, token->start);
        }
        break;
    }

}

