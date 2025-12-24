#include <stdio.h>
#include <stdlib.h>
#include "calc.h"


#define MAX_LENGTH 100


int main(void) {
    int type;
    double op2;
    char s[MAX_LENGTH];

    while ((type = getop(s)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(s));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '*':
                push(pop() * pop());
                break;
            case '/':
                op2 = pop();
                push (pop() / op2);
                break;
            case '\n':
                printf("%.2f\n", pop());
                break;
            default:
                break;
        }
    }
    return 0;
}
