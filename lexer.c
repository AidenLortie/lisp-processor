#include "lexer.h"
#include "library.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


Token* lex(char* input) {
    Token* head = NULL;
    Token* current = NULL;
    char* currentChar = input;

    unsigned long long inputLength = strlen(input);
    int index = 0;

    while (index < inputLength) {
        char currentCharValue = *currentChar;
        switch (currentCharValue) {
            case '(':
                current = addToken(&head, current, TOKEN_LPAREN, "(");
                index++;
                break;
            case ')':
                current = addToken(&head, current, TOKEN_RPAREN, ")");
                index++;
                break;
            case ' ':
                index++;
                break;
            case '"': {
                char value[256]; // Use stack buffer
                int valueIndex = 0;
                index++;
                currentChar = input + index;
                currentCharValue = *currentChar;
                while (currentCharValue != '"' && index < inputLength) {
                    value[valueIndex++] = currentCharValue;
                    index++;
                    currentChar = input + index;
                    currentCharValue = *currentChar;
                }
                value[valueIndex] = '\0';
                current = addToken(&head, current, TOKEN_STRING, value);
                index++;
                break;
            }
            default: {
                char value[256]; // Use stack buffer
                int valueIndex = 0;
                while (currentCharValue != ' ' && currentCharValue != '(' && currentCharValue != ')' &&
                       index < inputLength) {
                    value[valueIndex++] = currentCharValue;
                    index++;
                    currentChar = input + index;
                    currentCharValue = *currentChar;
                }
                value[valueIndex] = '\0';

                int isNumber = 1;
                for (int i = 0; i < strlen(value); i++) {
                    if (!isdigit(value[i])) {
                        isNumber = 0;
                        break;
                    }
                }

                if (isNumber) {
                    current = addToken(&head, current, TOKEN_NUMBER, value);
                } else {
                    current = addToken(&head, current, TOKEN_IDENTIFIER, value);
                }
                break;
            }
        }

        currentChar = input + index;
    }

    return head;  // <-- THIS is why it was crashing.
}


Token* addToken(Token** head, Token* current, TokenType type, char* value){
    Token* newToken = (Token*)malloc(sizeof(Token));
    newToken->type = type;
    newToken->value = strdup(value);
    newToken->next = NULL;

    if(*head == NULL){
        *head = newToken;
        current = newToken;
    }else{
        current->next = newToken;
        current = newToken;
    }

    return current;
}

void freeTokens(Token* head){
    Token* current = head;
    while(current != NULL){
        Token* next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}

void printTokens(Token* head){
    Token* current = head;
    while(current != NULL){
        printf("Token: %s\n", current->value);
        current = current->next;
    }
}

int getOperatorCode(const char* ident) {
    if (strcmp(ident, "+") == 0) return ADD;
    if (strcmp(ident, "-") == 0) return SUB;
    if (strcmp(ident, "*") == 0) return MUL;
    if (strcmp(ident, "/") == 0) return DIV;
    if (strcmp(ident, "def") == 0) return DEF;
    if (strcmp(ident, "seq") == 0) return SEQ;
    if (strcmp(ident, "if") == 0) return IF;
    if (strcmp(ident, ">") == 0) return GT;
    if (strcmp(ident, "<") == 0) return LT;
    if (strcmp(ident, "=") == 0) return EQ;
    if (strcmp(ident, ">=") == 0) return GTE;
    if (strcmp(ident, "<=") == 0) return LTE;
    if (strcmp(ident, "and") == 0) return AND;
    if (strcmp(ident, "or") == 0) return OR;
    if (strcmp(ident, "not") == 0) return NOT;
    if (strcmp(ident, "print") == 0) return PRINT;
    if (strcmp(ident, "input") == 0) return INPUT;
    fprintf(stderr, "Unknown operator '%s'\n", ident);
    exit(1);
}


Node* parseExpr(Token** current) {
    if (*current == NULL) {
        fprintf(stderr, "Unexpected end of tokens.\n");
        exit(1);
    }

    Token* token = *current;

    if (token->type != TOKEN_LPAREN) {
        fprintf(stderr, "Expected '(', got '%s'\n", token->value);
        exit(1);
    }

    // Advance to next token after '('
    *current = token->next;
    token = *current;

    if (token == NULL || token->type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected operator after '(', got '%s'\n", token ? token->value : "NULL");
        exit(1);
    }

    // Create the operator node (like ADD, DEF, etc.)
    int op = getOperatorCode(token->value);  // You'll need this function
    Node* node = createOperatorNode(op);

    // Advance to the first argument
    *current = token->next;
    token = *current;

    // Loop over arguments until we hit ')'
    while (token != NULL && token->type != TOKEN_RPAREN) {

        Node* child = NULL;

        if (token->type == TOKEN_LPAREN) {
            // Recursively parse the nested expression
            child = parseExpr(current);
        } else if (token->type == TOKEN_NUMBER) {
            child = createValueNode(strtol(token->value, NULL, 10));
            *current = token->next;
        } else if (token->type == TOKEN_IDENTIFIER) {
            child = createVariableNode(token->value);
            *current = token->next;
        } else if (token->type == TOKEN_STRING) {
            child = createStringLiteralNode(token->value);
            *current = token->next;
        } else {
            fprintf(stderr, "Unexpected token '%s'\n", token->value);
            exit(1);
        }

        addNode(node, child);
        token = *current;
    }

    if (token == NULL || token->type != TOKEN_RPAREN) {
        fprintf(stderr, "Expected ')', got '%s'\n", token ? token->value : "NULL");
        exit(1);
    }

    // Advance past ')'
    *current = token->next;

    return node;
}

Node* parse(Token* tokens) {
    Node* root = createOperatorNode(SEQ);

    Token* current = tokens;

    while(current != NULL){
        if(current->type == TOKEN_LPAREN){
            Node* expr = parseExpr(&current);
            addNode(root, expr);
        } else {
            fprintf(stderr, "Unexpected token '%s'\n", current->value);
            exit(1);
        }
    }

    return root;
}

