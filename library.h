#ifndef LISP_LITE_LIBRARY_H
#define LISP_LITE_LIBRARY_H

enum operators {
    ADD,
    SUB,
    MUL,
    DIV,
    DEF,
    SEQ,
    IF,
    GT,
    LT,
    EQ,
    GTE,
    LTE,
    AND,
    OR,
    NOT,
    PRINT,
    INPUT
};

typedef enum {
    VAL_INT,
    VAL_STRING
}ValueType;

typedef struct {
    ValueType type;
    union {
        int intValue;
        char *strValue;
    };
}Value;

typedef struct EnvEntry {
    char* name;
    Value value;
    struct EnvEntry* next;
}EnvEntry;

typedef enum{
    NODE_OPERATOR,
    NODE_VALUE,
    NODE_VARIABLE,
    NODE_STRING_LITERAL
} NodeType;


/*
 * Trees kinda look like:
 *          [+]
 *         /
 *       [1] -> [2] -> [*] (All children of +, however only 1 is directly referenced by +, the rest are "siblings" of 1 (via nextNode))
 *                     /
 *                   [3] -> [4]
 *
 */
typedef struct Node Node;
struct Node{
    NodeType type;
    union {
        int value;
        enum operators op;
        char* strValue;
    } val;
    Node *childNode;
    Node *nextNode;
};

Node *createNode(NodeType type, int value);

Node *createOperatorNode(int value);

Node *createValueNode(int value);

Node *createVariableNode(char* name);

Node *createStringLiteralNode(char* value);

void addNode(Node *parent, Node *child);

void printHelper(Node *node, char* prefix, int isLastChild);

void printTree(Node *node);

void freeTree(Node *node);

char* getOperatorSymbol(int operator);

Value evaluateTree(Node *node, EnvEntry **globalEnv);

Value env_get(EnvEntry* env, const char* name);

void env_set(EnvEntry** env, const char* name, Value value);

void env_free(EnvEntry* env);

Value makeIntValue(int value);

Value makeStringValue(const char* value);

#endif //LISP_LITE_LIBRARY_H
