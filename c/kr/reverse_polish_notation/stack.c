#include <stdio.h>
#include "calc.h"

#define STACK_SIZE 100

static double stack[STACK_SIZE];  /* visible only in file */
static long int pointer = 0;


void push(double num) {
    extern double stack[];  /* declaration is not needed */
    extern long int pointer;
    if (pointer < STACK_SIZE)
        stack[pointer++] = num;
    else
        printf("Stack is full\n");
    return;
}


double pop(void) {
    if (pointer > 0)
        return stack[--pointer];
    else {
        printf("Stack is empty\n");
        return 0.0;
    }
}
