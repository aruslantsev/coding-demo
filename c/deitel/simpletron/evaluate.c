#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "evaluate.h"

#define MAX_TOKENS      255

#define DEBUG

bool isArithmeticOp(const char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}


bool isBrace(const char c) {
    return c == '(' || c == ')';
}


int cmpOperations(char op1, char op2) {
    if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/' || op2 == '%')) return -1;
    if ((op1 == '*' || op1 == '/' || op2 == '%') && (op2 == '+' || op2 == '-')) return 1;
    return 0;
}


struct exprToken *tokenizeExpression(char expression[]) {
    /* Add trailing space. Last token will be saved automatically */
    strcat(expression, " ");

    struct exprToken *start = NULL, *currentToken;
    char currentIdentifier[TOKEN_SIZE];
    size_t tokenIndex = 0;
    const char *c = expression;
    while (*c != '\0') {
        /* Operation, brace, blank, end of line == token separator */
        if (isBrace(*c) || isArithmeticOp(*c) || isblank(*c) || *c == '\n' || *c == '\0') {
            /* Something was read during previous step */
            if (tokenIndex > 0) {
                /* Save token */
                currentIdentifier[tokenIndex] = '\0';
                struct exprToken *token = (struct exprToken *) malloc(sizeof(struct exprToken));
                strcpy(token->token, currentIdentifier);
#ifdef DEBUG
                printf("Adding token %s\n", token->token);
#endif
                token->token_type = IDENTIFIER;
                tokenIndex = 0;
                if (start == NULL) {
                    start = token;
                    currentToken = start;
                } else {
                    currentToken->next = token;
                    currentToken = currentToken->next;
                }
            }
            if (isBrace(*c) || isArithmeticOp(*c)) {
                /* Save braces and operations: read single symbol to token */
                struct exprToken *token = (struct exprToken *) malloc(sizeof(struct exprToken));
                token->token[0] = *c;
                token->token[1] = '\0';
                if (isArithmeticOp(*c)) token->token_type = OPERATION;
                else token->token_type = BRACE;
#ifdef DEBUG
                printf("Adding token %s\n", token->token);
#endif
                if (start == NULL) {
                    start = token;
                    currentToken = start;
                } else {
                    currentToken->next = token;
                    currentToken = currentToken->next;
                }
            }
            c++;
        } else {
            while (!(isBrace(*c) || isArithmeticOp(*c) || isblank(*c) || *c == '\n' || *c == '\0')) {
                /* Symbol. Save to token */
                currentIdentifier[tokenIndex++] = *c;
                c++;
            }
        }
    }
    if (transformAndCheckExpression(start)) {
        return toPostfix(start);
    }
    return NULL;
}


bool transformAndCheckExpression(struct exprToken *exprStart) {
    int bracesCount = 0;
    struct exprToken *currentToken;
    currentToken = exprStart;
    int tokencnt = 0;
    /* Leading unary operator */
    if (currentToken != NULL && currentToken->token_type == OPERATION) {
        if (currentToken->token[0] == '+' || currentToken->token[0] == '-') {
            currentToken->token_type = IDENTIFIER;
            currentToken->token[1] = '1';
            currentToken->token[2] = '\0';
            struct exprToken *tmpToken = (struct exprToken *) malloc(sizeof(struct exprToken));
            tmpToken->token_type = OPERATION;
            strcpy(tmpToken->token, "*");
            tmpToken->next = currentToken->next;
            currentToken->next = tmpToken;
        } else return false;
    }
    /* Other tokens */
    while (currentToken != NULL) {
#ifdef DEBUG
        printf("Got token '%s' with type '%c'. Token number %d\n", currentToken->token, currentToken->token_type, tokencnt);
#endif
        if (tokencnt > MAX_TOKENS) return false;
        
        /* Check braces */
        if (currentToken->token_type == BRACE) {
            if (currentToken->token[0] == '(') bracesCount++;
            else {
                bracesCount--;
                if (bracesCount < 0) return false;
            }
        }

        /* Two identifiers without operation between them */
        if (currentToken->token_type == IDENTIFIER) 
            if (currentToken->next != NULL && currentToken->next->token_type == IDENTIFIER) return false;

        if (currentToken->token_type == OPERATION) {
            if (currentToken->next != NULL && currentToken->next->token_type == OPERATION) {
                /* Three operations without identifiers or two trailing operations */
                if (currentToken->next->next == NULL || currentToken->next->next->token_type == OPERATION) return false;
                /* Two operations without identifiers: second can be only unary + or unary - */
                if (currentToken->next->token[0] == '+' || currentToken->next->token[0] == '-') {
                    // transform unary - or unary + to (-1 * ...) or (+1 * ...)
                    currentToken->next->token[1] = '1';
                    currentToken->next->token[2] = '\0';
                    currentToken->next->token_type = IDENTIFIER;
                    struct exprToken *tmpToken = (struct exprToken *) malloc(sizeof(struct exprToken));
                    tmpToken->token_type = OPERATION;
                    strcpy(tmpToken->token, "*");
                    tmpToken->next = currentToken->next->next;
                    currentToken->next->next = tmpToken;
                } else return false;
            }
        }
        currentToken = currentToken->next;
        tokencnt++;
    }
    return bracesCount == 0;
}


struct exprToken *toPostfix(struct exprToken *exprStart) {
    struct exprToken *postfixExpr = NULL;
    struct exprToken *currentToken = NULL;
    struct exprToken *tmpToken = NULL;
    struct exprToken *operationStack[MAX_TOKENS];
    size_t stackPtr = 0;

    while (exprStart != NULL) {
#ifdef DEBUG
        printf("Token %s %c\n", exprStart->token, exprStart->token_type);
#endif
        if (exprStart->token_type == IDENTIFIER) {
#ifdef DEBUG
            puts("To output queue");
#endif
            /* Add identifier to queue */
            if (postfixExpr == NULL) {
                postfixExpr = exprStart;
                currentToken = exprStart;
                exprStart = exprStart->next;
                currentToken->next = NULL;
            } else {
                currentToken->next = exprStart;
                currentToken = currentToken->next;
                exprStart = exprStart->next;
                currentToken->next = NULL;
            }
        } else if (exprStart->token_type == OPERATION) {
            while (
                stackPtr > 0
                && operationStack[stackPtr - 1]->token_type == OPERATION
                && cmpOperations(exprStart->token[0], operationStack[stackPtr - 1]->token[0]) <= 0
            ) {
#ifdef DEBUG
                puts("From stack");
#endif
                currentToken->next = operationStack[--stackPtr];
                currentToken = currentToken->next;
                currentToken->next = NULL;
            }
#ifdef DEBUG
            puts("To stack");
#endif
            operationStack[stackPtr++] = exprStart;
            exprStart = exprStart->next;
            operationStack[stackPtr - 1]->next = NULL;
        } else if (exprStart->token_type == BRACE && exprStart->token[0] == '(') {
#ifdef DEBUG
            puts("To stack (opening brace)");
#endif
            operationStack[stackPtr++] = exprStart;
            exprStart = exprStart->next;
            operationStack[stackPtr - 1]->next = NULL;
        } else if (exprStart->token_type == BRACE  && exprStart->token[0] == ')') {
            while (
                stackPtr > 0 && operationStack[stackPtr - 1]->token_type != BRACE
            ) {
#ifdef DEBUG
                puts("From stack (closing brace)");
#endif
                currentToken->next = operationStack[--stackPtr];
                currentToken = currentToken->next;
                currentToken->next = NULL;
            }
            puts("Pop bracket");
            tmpToken = operationStack[--stackPtr];
            free(tmpToken);
            tmpToken = exprStart;
            exprStart = exprStart->next;
            free(tmpToken);
        }
    }
    while (stackPtr > 0) {
#ifdef DEBUG
        puts("From stack (end of expr)");
#endif
        currentToken->next = operationStack[--stackPtr];
        currentToken = currentToken->next;
        currentToken->next = NULL;
    }
#ifdef DEBUG
    int tokenCnt = 0;
    currentToken = postfixExpr;
    puts("Postfix expression");
    while (currentToken != NULL) {
        printf("%s ", currentToken->token);
        currentToken = currentToken->next;
        tokenCnt++;
        if (tokenCnt > MAX_TOKENS) exit(1);
    }
    puts("");
#endif
    return postfixExpr;
}
