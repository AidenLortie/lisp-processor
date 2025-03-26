#ifndef LISP_LITE_LEXER_H
#define LISP_LITE_LEXER_H
#include "library.h"

typedef enum {
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_STRING
} TokenType;

typedef struct Token {
    TokenType type;
    char* value;
    struct Token* next;
} Token;

Token* lex(char* input);

Token* addToken(Token** head, Token* current, TokenType type, char* value);

void freeTokens(Token* head);

void printTokens(Token* head);

Node *parseExpr(Token** head);

Node* parse(Token* tokens);



#endif //LISP_LITE_LEXER_H
