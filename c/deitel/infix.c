#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define MAXLEN 1000

bool isop(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int cmpop(char a, char b) {
    if ((a == '+' || a == '-') && (b == '*' || b == '/')) {
        return -1;
    } else if ((a == '*' || a == '/') && (b == '+' || b == '-')) {
        return 1;
    } else {
        return 0;
    }
}

int main(void) {
    char postfix[MAXLEN], infix[MAXLEN];
    char stack[MAXLEN];

    size_t ip = 0, pp = 0, sp = 0;

    gets(infix);  /* C99 */

    while (infix[ip] != '\0') {
        if (infix[ip] == ' ') {
            if (postfix[pp] != ' ') postfix[pp++] = ' ';
            while (infix[ip] == ' ') ip++;
        } else if (
            isdigit(infix[ip])
            || infix[ip] == '.'
            || (infix[ip] == '-' && isdigit(infix[ip + 1]))
        ) {
            while (
                isdigit(infix[ip])
                || infix[ip] == '.'
                || (infix[ip] == '-' && isdigit(infix[ip + 1]))
            ) {
                postfix[pp++] = infix[ip++];
            }
        } else if (isop(infix[ip])) {
            /* operator on top of the stack, incomint has lower or equal priority */
            while (sp > 0 && isop(stack[sp - 1]) && cmpop(infix[ip], stack[sp - 1]) <= 0) {
                postfix[pp++] = stack[--sp];
                if (postfix[pp] != ' ') postfix[pp++] = ' ';
            }
            stack[sp++] = infix[ip++];
        } else if (infix[ip] == '(') {
            stack[sp++] = infix[ip++];
        } else if (infix[ip] == ')') {
            while (stack[sp - 1] != '(') {
                postfix[pp++] = stack[--sp];
                if (postfix[pp] != ' ') postfix[pp++] = ' ';
            }
            --sp;  /* pop ( */
        }
    }
    if (postfix[pp] != ' ') postfix[pp++] = ' ';
    while (sp > 0) {
        postfix[pp++] = stack[--sp];
        if (postfix[pp] != ' ') postfix[pp++] = ' ';
    }

    postfix[pp] = '\0';
    printf("%s\n", postfix);
    return 0;
}
