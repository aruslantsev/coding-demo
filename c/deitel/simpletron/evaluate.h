#pragma once

#include <stdbool.h>

#define TOKEN_SIZE  20

enum tokenType {
    IDENTIFIER='i', OPERATION='o', BRACE='b'
};

struct exprToken {
    char token[TOKEN_SIZE];
    char token_type;
    struct exprToken *next;
};


struct exprToken *tokenizeExpression(char []);
bool transformAndCheckExpression(struct exprToken *);
struct exprToken *toPostfix(struct exprToken *);

int cmpOperations(char, char);
bool isArithmeticOp(const char);
bool isBrace(const char);
