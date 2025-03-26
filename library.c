#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"



/**
 * @brief Generate a new operator node
 * @param value The operator value (from enum operators)
 * @return The new operator node
 */
Node* createOperatorNode(int value){
    Node *node =  createNode(NODE_OPERATOR, value);
    return node;
}

/**
 * @brief Generate a new value node
 * @param value The value of the node
 * @return The new value node
 */
Node* createValueNode(int value){
    Node *node =  createNode(NODE_VALUE, value);
    return node;
}

/**
 * @brief create a variable node
 * @param name
 * @return
 */
Node* createVariableNode(char* name){
    Node *node = createNode(NODE_VARIABLE, 0);
    node->val.strValue = (char*)malloc(strlen(name) + 1);
    strcpy(node->val.strValue, name);
    return node;
}

/**
 * @brief Generate a new node
 * @param type The type of the node
 * @param value The value of the node
 * @return The new node
 */
Node *createNode(NodeType type, int value){
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = type;
    switch(type){
        case NODE_OPERATOR:
            node->val.op = value;
            break;
        case NODE_VALUE:
            node->val.value = value;
            break;
        case NODE_VARIABLE:
            node->val.strValue = NULL;
            break;
        default:
            break;
    }
    node->childNode = NULL;
    node->nextNode = NULL;
    return node;
}

/**
 * @brief Add a child node to a parent node
 * @param parent The parent node
 * @param child The child node
 */
void addNode(Node *parent, Node *child){
    if(parent->childNode == NULL){
        parent->childNode = child;
    }else{
        Node *current = parent->childNode;
        while(current->nextNode != NULL){
            current = current->nextNode;
        }
        current->nextNode = child;
    }
}

/**
 * @brief Print a tree
 * @param node The root node of the tree
 * @param prefix The prefix to print
 * @param isLastChild Whether the node is the last child
 */
void printHelper(Node *node, char* prefix, int isLastChild){


    if(node == NULL){
        return;
    }
    printf(COLOR_CYAN "%s", prefix);

    if(node->type == NODE_OPERATOR){
        printf("|--+ " COLOR_BLUE "%s\n" COLOR_RESET, getOperatorSymbol(node->val.op));
    }else if (node->type == NODE_VARIABLE){
        if(!isLastChild) {
            printf("|-- " COLOR_YELLOW "%s\n" COLOR_RESET, node->val.strValue);
        } else {
            printf("\\-- " COLOR_YELLOW "%s\n" COLOR_RESET, node->val.strValue);
        }
    }else{
        if(!isLastChild){
            printf("|-- " COLOR_GREEN "%d\n" COLOR_RESET, node->val.value);
        }else{
            printf("\\-- " COLOR_GREEN "%d\n" COLOR_RESET, node->val.value);
        }
    }
    char newPrefix[255];
    sprintf(newPrefix, "%s|  ", prefix);


    if (node->childNode != NULL) {
        printHelper(node->childNode, newPrefix, node->nextNode == NULL ? 1 : 0);
    }

    if (node->nextNode != NULL) {
        printHelper(node->nextNode, prefix, node->nextNode->nextNode == NULL ? 1 : 0);
    }


}

/**
 * @brief Print a tree
 * @param node The root node of the tree
 */
void printTree(Node *node){
    printHelper(node, "", 0);
}

/**
 * @brief Free a tree
 * @param node
 */
void freeTree(Node *node){
    if(node == NULL) return;

    freeTree(node->childNode);
    freeTree(node->nextNode);

    free(node);
}

/**
 * @brief Get the operator symbol
 * @param operator The operator value
 * @return The operator symbol
 */
char* getOperatorSymbol(int operator){
    switch(operator){
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case SEQ:
            return "SEQ";
        case DEF:
            return "DEF";
        case IF:
            return "IF";
        case GT:
            return "GT";
        case LT:
            return "LT";
        case EQ:
            return "EQ";
        case GTE:
            return "GTE";
        case LTE:
            return "LTE";
        case AND:
            return "AND";
        case OR:
            return "OR";
        case NOT:
            return "NOT";
        case PRINT:
            return "PRINT";
        default: {
            static char buf[16];
            snprintf(buf, sizeof(buf), "OP_%d", operator);
            return buf;
        }

    }
}

/**
 * @brief Evaluate a tree
 * @param node The root node of the tree
 * @param globalEnv The global environment
 * @return The result of the evaluation
 */
Value evaluateTree(Node *node, EnvEntry **globalEnv){
    if(node == NULL){
        return (Value){.type = VAL_INT, .intValue = 0};
    }

    if(node->type == NODE_VALUE){
        return (Value){.type = VAL_INT, .intValue = node->val.value};
    }

    if(node->type == NODE_VARIABLE){
        return env_get(*globalEnv, node->val.strValue);
    }

    if(node->type == NODE_STRING_LITERAL){
        return (Value){.type = VAL_STRING, .strValue = node->val.strValue};
    }

    int result = 0;
    Node *current = node->childNode;
    switch(node->val.op){
        case ADD: {
            while (current != NULL) {
                if (current->type == NODE_OPERATOR) {
                    result += evaluateTree(current, globalEnv).intValue;
                    current = current->nextNode;
                    continue;
                }

                result += evaluateTree(current, globalEnv).intValue;
                current = current->nextNode;

            }
            break;
        }
        case SUB:
        {
            result = evaluateTree(current, globalEnv).intValue;
            current = current->nextNode;
            while (current != NULL) {
                if (current->type == NODE_OPERATOR) {
                    result -= evaluateTree(current, globalEnv).intValue;
                    current = current->nextNode;
                    continue;
                }

                result -= evaluateTree(current, globalEnv).intValue;
                current = current->nextNode;
            }
            break;
        }
        case MUL:{
            result = 1;
            while (current != NULL) {
                if (current->type == NODE_OPERATOR) {
                    result *= evaluateTree(current, globalEnv).intValue;
                    current = current->nextNode;
                    continue;
                }

                result *= evaluateTree(current, globalEnv).intValue;
                current = current->nextNode;
            }
            break;
        }
        case DIV: {
            result = evaluateTree(current, globalEnv);
            current = current->nextNode;
            while (current != NULL) {
                if (current->type == NODE_OPERATOR) {
                    result /= evaluateTree(current, globalEnv);
                    current = current->nextNode;
                    continue;
                }

                result /= evaluateTree(current, globalEnv);
                current = current->nextNode;
            }
            break;
        }
        case DEF: {
            Node* varNode = node->childNode;
            Node* exprNode = varNode->nextNode;

            if(varNode == NULL || varNode->type != NODE_VARIABLE){
                fprintf(stderr, "Error: Expected variable name\n");
                exit(1);
            }

            if(exprNode == NULL){
                fprintf(stderr, "Error: Expected expression\n");
                exit(1);
            }

            int value = evaluateTree(exprNode, globalEnv);
            env_set(globalEnv, varNode->val.strValue, value);
            return value;
        }
        case SEQ: {
            while (current != NULL) {
                result = evaluateTree(current, globalEnv);
                current = current->nextNode;
            }
            break;
        }
        case IF: {
            if(current == NULL || current->nextNode == NULL || current->nextNode->nextNode == NULL){
                fprintf(stderr, "Error: Expected three arguments for IF\n");
                exit(1);
            }

            Node* condition = current;
            Node* trueBranch = current->nextNode;
            Node* falseBranch = trueBranch->nextNode;

            if(evaluateTree(condition, globalEnv)){
                result = evaluateTree(trueBranch, globalEnv);
            }else{
                result = evaluateTree(falseBranch, globalEnv);
            }
            break;
        }
        case GT: {
            if(current == NULL || current->nextNode == NULL){
                fprintf(stderr, "Error: Expected two arguments for GT\n");
                exit(1);
            }

            int left = evaluateTree(current, globalEnv);
            int right = evaluateTree(current->nextNode, globalEnv);
            result = left > right;
            break;
        }
        case LT: {
            if(current == NULL || current->nextNode == NULL){
                fprintf(stderr, "Error: Expected two arguments for LT\n");
                exit(1);
            }

            int left = evaluateTree(current, globalEnv);
            int right = evaluateTree(current->nextNode, globalEnv);
            result = left < right;
            break;
        }
        case EQ: {
            if(current == NULL || current->nextNode == NULL){
                fprintf(stderr, "Error: Expected two arguments for EQ\n");
                exit(1);
            }

            int left = evaluateTree(current, globalEnv);
            int right = evaluateTree(current->nextNode, globalEnv);
            result = left == right;
            break;
        }
        case GTE: {
            if(current == NULL || current->nextNode == NULL){
                fprintf(stderr, "Error: Expected two arguments for GTE\n");
                exit(1);
            }

            int left = evaluateTree(current, globalEnv);
            int right = evaluateTree(current->nextNode, globalEnv);
            result = left >= right;
            break;
        }
        case LTE: {
            if(current == NULL || current->nextNode == NULL){
                fprintf(stderr, "Error: Expected two arguments for LTE\n");
                exit(1);
            }

            int left = evaluateTree(current, globalEnv);
            int right = evaluateTree(current->nextNode, globalEnv);
            result = left <= right;
            break;
        }
        case AND: {
            if(current == NULL || current->nextNode == NULL){
                fprintf(stderr, "Error: Expected two arguments for AND\n");
                exit(1);
            }

            int left = evaluateTree(current, globalEnv);
            int right = evaluateTree(current->nextNode, globalEnv);
            result = left && right;
            break;
        }
        case OR: {
            if(current == NULL || current->nextNode == NULL){
                fprintf(stderr, "Error: Expected two arguments for OR\n");
                exit(1);
            }

            int left = evaluateTree(current, globalEnv);
            int right = evaluateTree(current->nextNode, globalEnv);
            result = left || right;
            break;
        }
        case NOT: {
            if(current == NULL){
                fprintf(stderr, "Error: Expected one argument for NOT\n");
                exit(1);
            }

            result = !evaluateTree(current, globalEnv);
            break;
        }
        case PRINT :{
            while (current != NULL) {
                result = evaluateTree(current, globalEnv);
                printf("%d\n", result);
                current = current->nextNode;
            }
            break;
        }
        default:
            return 0;
    }

    return result;
}

/**
 * @brief Get a variable from the environment
 * @param env
 * @param name
 * @return
 */
Value env_get(EnvEntry* env, const char* name) {
    EnvEntry* current = env;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current->value;
        }
        current = current->next;
    }

    fprintf(stderr, "Error: Variable %s not found\n", name);
    exit(1);
}


/**
 * @brief Set a variable in the environment
 * @param env
 * @param name
 * @param value
 */
void env_set(EnvEntry** env, const char* name, int value) {
    EnvEntry* current = *env;

    while(current != NULL){
        if(strcmp(current->name, name) == 0){
            current->value = value;
            return;
        }
        current = current->next;
    }

    EnvEntry* newEntry = (EnvEntry*)malloc(sizeof(EnvEntry));
    newEntry->name = strdup(name); // malloc + strcpy in one
    newEntry->value = value;

    newEntry->next = *env;  // Add to front
    *env = newEntry;
}

void env_free(EnvEntry* env){
    EnvEntry* current = env;
    while(current != NULL){
        EnvEntry* next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

Value makeIntValue(int x){
    return (Value){.type = VAL_INT, .intValue = x};
}

Value makeStringValue(const char* s){
    Value v;
    v.type = VAL_STRING;
    v.strValue = strdup(s);
    return v;
}

