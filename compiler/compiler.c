#include <stdio.h>

#include "../compiler/compiler.h"
#include "../scanner/scanner.h"

typedef struct {
    Token previous;
    Token current;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;

static void advance();
static void errorAtCurrent(const char *message);
static void errorAtPrevious(const char *message);
static void errorAt(Token *token, const char *message);

bool compile(const char *source, Chunk *chunk) {
    initScanner(source);
    advance();
    //expression();
    //consume(TOKEN_EOF, "Expect end of expression");

    return !parser.hadError;
}


static void advance() {
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) continue;

        errorAtCurrent(parser.current.start);
    }
}

static void errorAtCurrent(const char *message) {
    errorAt(&parser.current, message);
}

static void errorAtPrevious(const char *message) {
    errorAt(&parser.previous, message);
}
static void errorAt(Token *token, const char *message) {
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

