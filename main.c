#include "library.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv){

    if(argc != 2){
        fprintf(stderr, "Usage: %s <input>\n", argv[0]);
        return 1;
    }

    char* input = argv[1];
    FILE *file = fopen(input, "r");
    if(file == NULL){
        fprintf(stderr, "Error: Could not open file %s\n", input);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);

    buffer[length] = '\0';

    for (char* p = buffer; *p; ++p) {
        if (*p == '\r') *p = ' ';
        if (*p == '\n') *p = ' ';
    }

    for (int i = 0; i < length; ++i) {
        if ((unsigned char)buffer[i] < 32 || (unsigned char)buffer[i] > 126) {
            // Replace non-printable ASCII chars with space or null
            buffer[i] = ' ';
        }
    }



    printf("Buffer: [%s]\n", buffer);

    printf("Opened file: %s\n", input);
    printf("File length: %ld\n", length);
    printf("Buffer (first 100 chars): %.100s\n", buffer);


    fclose(file);


    Token* tokens = lex(buffer);

    Node* AST = parse(tokens);

    printTree(AST);

    EnvEntry* env = NULL;

    Value result = evaluateTree(AST, &env);
    if(result.type == VAL_INT){
        printf("Result: %d\n", result.intValue);
    }else{
        printf("Result: %s\n", result.strValue);
    }

    freeTokens(tokens);
    freeTree(AST);
    env_free(env);

    return 0;
}

