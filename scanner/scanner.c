//
// Created by sascha-roggatz on 01.11.24.
//

#include "scanner.h"

#include <ctype.h>
#include <string.h>

Scanner scanner;

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


/**
 * Initializes the scanner to start scanning the provided source code.
 *
 * @param source The source code to be scanned.
 */
void initScanner(const char *source) {
    scanner.current = source;
    scanner.start = source;
    scanner.line = 1;
}

/**
 * Scans the next token from the source code and returns it.
 *
 * This function handles different token types such as parentheses, braces,
 * semicolons, operators, identifiers, numbers, and strings. It also skips
 * whitespace and comments. If an unexpected character is encountered, an
 * error token is returned.
 *
 * @return The next token in the source code.
 */
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

/**
 * Generates a token with the specified type.
 *
 * @param type The type of the token to be generated.
 * @return The constructed token with the given type.
 */
Token makeToken(const TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.line = scanner.line;
    token.length = (int) (scanner.current - scanner.start);
    return token;
}

/**
 * Creates a token for a string literal.
 *
 * @param type The type of the token.
 * @param startIdx The starting index of the string literal in the source code.
 * @param endIdx The ending index of the string literal in the source code.
 * @return A token representing the string literal.
 */
Token makeStringToken(TokenType type, const char *startIdx,
                      const char *endIdx) {
    Token token;
    token.type = type;
    token.start = startIdx;
    token.length = (int) (endIdx - startIdx);
    token.line = scanner.line;

    return token;
}

/**
 * Creates a token representing an error with the provided error message.
 *
 * @param message The error message to associate with the error token.
 * @return A Token object of type TOKEN_ERROR, initialized with the current
 *         position and line number in the source code, and the length of the error message.
 */
Token errorToken(const char *message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = scanner.start;
    token.line = scanner.line;
    token.length = (int) strlen(message);
    return token;
}

/**
 * Reads a string token from the source code.
 *
 * This function continues scanning characters until it encounters a closing
 * double-quote character or the end of the source code. If a newline character
 * is encountered within the string, it increments the line count.
 * If the end of the source code is reached without finding a closing quote,
 * an error token for an unterminated string is returned.
 *
 * @return A Token representing the string, or an error token if the string is unterminated.
 */
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

/**
 * Reads an identifier from the source code.
 *
 * This function advances through characters, as long as they satisfy the
 * condition of being alphabetic (a-z, A-Z) and the end of the source code
 * has not been reached.
 *
 * @return A token representing the identifier.
 */
Token readIdentifier() {
    while (isalpha(peek()) && !isAtEnd()) advance();
    return makeToken(identifierType());
}

/**
 * Determines the type of identifier based on the initial characters.
 *
 * The function checks the first character of an identifier and uses
 * a switch statement to compare it against known keywords. If the
 * identifier matches one of these keywords, the corresponding token type
 * is returned.
 *
 * @return The token type corresponding to the identifier or TOKEN_IDENTIFIER if it doesn't match a keyword.
 */
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

/**
 * Checks if the current token matches a specific keyword.
 *
 * @param offset The offset in the current token where comparison starts.
 * @param len_suffix The length of the suffix to compare.
 * @param suffix The suffix to compare with the current token.
 * @param type The token type to return if the keyword matches.
 * @return The specified keyword token type if matched; otherwise, TOKEN_IDENTIFIER.
 */
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

/**
 * Checks if the scanner has reached the end of the source code.
 *
 * @return True if the scanner is at the end of the source code,
 *         otherwise false.
 */
bool isAtEnd() {
    return *scanner.current == '\0';
}

/**
 * Checks if the next character in the source is the expected character.
 *
 * @param expected The character to be matched.
 * @return true if the next character matches the expected character, false otherwise.
 */
bool isNextToken(const char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;

    scanner.current++;
    return true;
}

/**
 * Reads a numeric value from the source code, recognizing both integers and floating-point numbers.
 *
 * The function continues to consume digits until it encounters a non-digit character. If a decimal
 * point is found and it is followed by more digits, the function treats this as a floating-point number.
 *
 * @return A token of type TOKEN_NUMBER.
 */
Token readNumber() {
    while (isdigit(peek())) advance();

    if (peek() == '.' && !isAtEnd()) {
        advance();
        while (isdigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

/**
 * Advances to the next character in the source code and returns the current character.
 *
 * @return The current character after advancing.
 */
char advance() {
    scanner.current++;
    return scanner.current[-1];
}

/**
 * Skips over any whitespace characters in the input source code.
 *
 * This function advances the scanner's current position past any spaces,
 * carriage returns, tabs, and new line characters. It updates the line count
 * for new line characters.
 */
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

/**
 * Peeks at the current character in the scanner without consuming it.
 *
 * @return The current character being pointed to by the scanner.
 */
static char peek() {
    return *scanner.current;
}
