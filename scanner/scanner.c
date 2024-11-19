//
// Created by sascha-roggatz on 01.11.24.
//

#include "scanner.h"

#include <ctype.h>
#include <string.h>

Token makeToken(TokenType type);

Token makeStringToken(TokenType type, const char *startIdx, const char *endIdx);

Token errorToken(const char *message);

Token readString();

Token readNumber();

Token readIdentifier();

static TokenType identifierType();

TokenType checkKeyword(int offset, int len_suffix, const char *suffix,
                       TokenType type);

bool isAtEnd();

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
            if (isNextToken('/')) {
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                return makeToken(TOKEN_SLASH);
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
            if (isdigit(c)) return readNumber();
            if (isalpha(c) || c == '_') return readIdentifier();
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

Token makeStringToken(TokenType type, const char *startIdx,
                      const char *endIdx) {
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

    const char *start = scanner.start;
    const char *end = scanner.current;

    return makeStringToken(
        TOKEN_STRING,
        &start[1],
        &end[-1]);
}

Token readIdentifier() {
    while (isalpha(peek()) && !isAtEnd()) advance();
    return makeToken(identifierType());
}

static TokenType identifierType() {
    const char c = *scanner.start;

    switch (c) {
        case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
        case 'f': {
            if (scanner.current - scanner.start >= 2) {
                switch (scanner.start[1]) {
                    case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
                }
            }
            break;
        }
        case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, 1, "o", TOKEN_OR);
        case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
        case 't': {
            if (scanner.current - scanner.start >= 2) {
                switch (scanner.start[1]) {
                    case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
                    case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
        }
        case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

TokenType checkKeyword(
    const int offset,
    const int len_suffix,
    const char *suffix,
    const TokenType type
) {
    if (scanner.current - scanner.start == offset + len_suffix &&
        memcmp(scanner.start + offset, suffix, len_suffix) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
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
    while (isdigit(peek())) advance();

    if (peek() == '.' && !isAtEnd()) {
        advance();
        while (isdigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
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
