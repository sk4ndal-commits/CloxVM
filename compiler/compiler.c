#include <stdio.h>

#include "../compiler/compiler.h"

#include <stdlib.h>

#include "../scanner/scanner.h"
#include "../enums/opcodes.h"

#ifdef DEBUG_PRINT_CODE
#include "../debug/debug.h"
#endif

typedef struct {
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
} Parser;

typedef enum {
    PRECEDENCE_NONE,
    PRECEDENCE_ASSIGNMENT,
    PRECEDENCE_OR,
    PRECEDENCE_AND,
    PRECEDENCE_EQUALITY,
    PRECEDENCE_COMPARISON,
    PRECEDENCE_TERM,
    PRECEDENCE_FACTOR,
    PRECEDENCE_UNARY,
    PRECEDENCE_CALL,
    PRECEDENCE_PRIMARY,
} Precedence;

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

static void grouping();

static void unary();

static void binary();

static void number();

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN]      = {grouping, NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_PAREN]     = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_LEFT_BRACE]      = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_BRACE]     = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_COMMA]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_DOT]             = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_MINUS]           = {unary, binary, PRECEDENCE_TERM},
    [TOKEN_PLUS]            = {NULL, binary, PRECEDENCE_TERM},
    [TOKEN_SEMICOLON]       = {NULL,NULL},
    [TOKEN_SLASH]           = {NULL, binary, PRECEDENCE_FACTOR},
    [TOKEN_STAR]            = {NULL, binary, PRECEDENCE_FACTOR},
    [TOKEN_BANG]            = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_BANG_EQUAL]      = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL_EQUAL]     = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER]         = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER_EQUAL]   = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_LESS]            = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_LESS_EQUAL]      = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_IDENTIFIER]      = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_STRING]          = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_NUMBER]          = {number,NULL, PRECEDENCE_NONE},
    [TOKEN_AND]             = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_CLASS]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_ELSE]            = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_FALSE]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_FOR]             = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_FUN]             = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_IF]              = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_NIL]             = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_OR]              = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_PRINT]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_RETURN]          = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_SUPER]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_THIS]            = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_TRUE]            = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_VAR]             = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_WHILE]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_ERROR]           = {NULL,NULL, PRECEDENCE_NONE},
    [TOKEN_EOF]             = {NULL,NULL, PRECEDENCE_NONE},
};

Parser parser;
Chunk *compilingChunk;


static Chunk *currentChunk();

static uint8_t makeConstant(Value value);

static void emitConstant(Value value);

static void emitByte(uint8_t byte);

static void emitBytes(uint8_t byte1, uint8_t byte2);

static void emitReturn();

static void endCompiler();

static void expression();

static ParseRule* getRule(TokenType operationType);

static void parsePrecedence(Precedence precedence);

static void advance();

static void consume(TokenType tokenType, const char *message);

static void errorAtCurrent(const char *message);

static void errorAtPrevious(const char *message);

static void errorAt(const Token *token, const char *message);

/**
 * Compiles the given source code string into the provided chunk.
 *
 * @param source The source code to compile.
 * @param chunk The chunk where the compiled bytecode will be stored.
 * @return true if compilation was successful, false if there were errors.
 */
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
    parsePrecedence(PRECEDENCE_ASSIGNMENT);
}

static void number() {
    const double value = strtod(parser.previous.start, NULL);
    emitConstant(value);
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

static uint8_t makeConstant(const Value value) {
    const int consIdx = addConstant(compilingChunk, value);
    if (consIdx > UINT8_MAX) {
        errorAtPrevious("Too many constants");
        return 0;
    }

    return consIdx;
}

static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression");
}

static void unary() {
    const TokenType operationType = parser.previous.type;

    parsePrecedence(PRECEDENCE_UNARY);

    switch (operationType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE);
            break;
        default: break;
    }
}

static void binary() {
    TokenType operationType = parser.previous.type;

    const ParseRule *rule = getRule(operationType);
    parsePrecedence(rule->precedence+1);

    switch (operationType) {
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: break;
    }
}

static void parsePrecedence(const Precedence precedence) {
    advance();
    const ParseFn prefixRule = getRule(parser.previous.type)->prefix;

    if (prefixRule == NULL) {
        errorAtPrevious("Expected a precedence rule");
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        const ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}

static ParseRule* getRule(const TokenType operationType) {
    return &rules[operationType];
}

static void endCompiler() {
    emitReturn();
}

static void emitReturn() {
    emitByte(OP_RETURN);
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        disassembleChunk(currentChunk(), "code");
    }
#endif
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

static Chunk *currentChunk() {
    return compilingChunk;
}

static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();

        if (parser.current.type == TOKEN_EOF) return;

        if (parser.current.type == TOKEN_ERROR) {
            errorAtCurrent(parser.current.start);
            return;
        }
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
