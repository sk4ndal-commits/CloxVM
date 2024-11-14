//
// Created by sascha-roggatz on 01.11.24.
//

#include "scanner.h"

#include <string.h>

Token makeToken(TokenType type);

Token makeStringToken(TokenType type, const char *startIdx, const char *endIdx);

Token errorToken(const char *message);

Token readString();

Token readNumber();

bool isAtEnd();

bool isDigit(char c);

char advance();

bool isNextToken(char expected);

void skipWhitespace();

static char peek();

Scanner scanner;

void initScanner(const char *source) {
    scanner.current = source;
    scanner.start = source;
    scanner.line = 1;
}

Token scanToken() {
    skipWhitespace();

    scanner.start = scanner.current;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    const char c = advance();


    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/':
            if (isNextToken('\n')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                return;
            }
            break;
        case '*': return makeToken(TOKEN_STAR);
        case '!': return makeToken(
                isNextToken('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(
                isNextToken('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(
                isNextToken('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(
                isNextToken('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return readString();
        default: {
            if (isDigit(c)) return readNumber();
        }
            break;
    }

    return errorToken("Unexpected character");
}

Token makeToken(const TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.line = scanner.line;
    token.length = (int) (scanner.current - scanner.start);
    return token;
}

Token makeStringToken(TokenType type, const char *startIdx, const char *endIdx) {
    Token token;
    token.type = type;
    token.start = startIdx;
    token.length = (int) (endIdx - startIdx);
    token.line = scanner.line;

    return token;
}

Token errorToken(const char *message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = scanner.start;
    token.line = scanner.line;
    token.length = (int) strlen(message);
    return token;
}

Token readString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string");

    advance();

    const char* start = scanner.start;
    const char* end = scanner.current;

    return makeStringToken(
        TOKEN_STRING,
        &(start[1]),
        &(end[-1]));


}

bool isAtEnd() {
    return *scanner.current == '\0';
}

bool isNextToken(const char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;

    scanner.current++;
    return true;
}

Token readNumber() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && !isAtEnd()) {
        advance();
        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

char advance() {
    scanner.current++;
    return scanner.current[-1];
}

void skipWhitespace() {
    for (;;) {
        const char c = peek();

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            default:
                return;
        }
    }
}

static char peek() {
    return *scanner.current;
}
