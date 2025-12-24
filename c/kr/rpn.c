#include <stdlib.h>
#include <stdio.h>

#define STACK_SIZE 100


typedef union {
    double num;
    int operation;
} val;


int s_len(char *s) {
    int res;
    for (res = 0; s[res] != '\0'; res++);
    return res;
}


double compute(double op1, double op2, int operation) {
    switch (operation) {
        case '+':
            return op1 + op2;
            break;
        case '-':
            return op1 - op2;
            break;
        case '*':
            return op1 * op2;
            break;
        case '/':
            if (op2 == 0.0) {
              printf("Zero division\n");
              exit(1);
            }
            return op1 / op2;
            break;
        default:
            printf("Illegal operation: %c. Exiting\n", operation);
            exit(1);
    }
    return 0;
}


double stof(char *s) {
    int sp = 0, num_delimiters = 0, sign, power = 1;
    double res = 0;
    sign = s[sp] == '-'? -1: 1;
    if (s[sp] == '-' || s[sp] == '+') {
        sp++;
    }
    while (s[sp] != '\0') {
        if ((s[sp] - '0' >= 0 && s[sp] - '0' <= 9) || s[sp] == '.') {
            if (s[sp] != '.') {
                res = res * 10. + (s[sp++] - '0');
                if (num_delimiters > 0) {
                    power *= 10;
                }
            } else {
                if (num_delimiters == 0) {
                    num_delimiters++;
                    sp++;
                } else {
                    printf("Incorrect number: %s\n", s);
                    exit(1);
                }
            }
        } else {
            printf("Incorrect number: %s\n", s);
            exit(1);
        }
    }
    return res * sign / power;
}


int get_op(char *s, val *res) {
    if ((s_len(s) == 1) && !((*s - '0' >= 0) && (*s - '0' <= 9))) {
        res -> operation = *s;
        return 1;
    } else {
        res -> num = stof(s);
        return 0;
    }
}


int main(int argc, char *argv[]) {
    int sp = 0, op_type;
    val retval;
    double stack[STACK_SIZE], op1, op2;
    while (--argc > 0) {
        op_type = get_op(*(++argv), &retval);
        if (op_type == 0) {
            stack[sp++] = retval.num;
        } else if (op_type == 1) {
            if (sp < 2) {
                printf("Error: not enough records in stack\n");
                exit(1);
            }
            op2 = stack[--sp];
            op1 = stack[--sp];
            stack[sp++] = compute(op1, op2, retval.operation);
        }
#ifdef DEBUG
        printf("Stack: ");
        for (int csp = 0; csp < sp; csp++) {
            printf("%.2f ", stack[csp]);
        }
        printf("\n");
#endif
    }
    if (sp != 1) {
        printf("Number of records in stack: %d, should be 1\n", sp);
        exit(1);
    }
    printf("Result: %.2f\n", stack[--sp]);
    return 0;
}
