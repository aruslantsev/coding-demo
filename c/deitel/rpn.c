#include <stdio.h>
#include <ctype.h>

#define MAX_LENGTH      100

int main(void) {
    char expr[MAX_LENGTH];
    double stack[MAX_LENGTH];
    double num = 0, tmpnum = 0;
    int sign, divisor;
    size_t sp = 0, ep = 0;

    gets(expr);  /* C99 */

    while (expr[ep] != '\0') {
        if (expr[ep] == ' ') {
            ep++;
        } else if (
            isdigit(expr[ep])
            || expr[ep] == '.'
            || (expr[ep] == '-' && isdigit(expr[ep + 1]))
        ) {
            num = 0;
            sign = 1;
            divisor = 1;
            if (expr[ep] == '-') {
                sign = -1;
                ep++;
            }
            while (isdigit(expr[ep])) {
                num = num * 10 + (expr[ep++] - '0');
            }
            if (expr[ep] == '.') ep++;
            while (isdigit(expr[ep])) {
                num = num * 10 + (expr[ep++] - '0');
                divisor *= 10;
            }
            stack[sp++] = num * sign / divisor;
        } else if (expr[ep] == '+') {
            num = stack[--sp];
            num += stack[--sp];
            stack[sp++] = num;
            ep++;
        } else if (expr[ep] == '-') {
            num = -stack[--sp];
            num += stack[--sp];
            stack[sp++] = num;
            ep++;
        } else if (expr[ep] == '*') {
            num = stack[--sp];
            num *= stack[--sp];
            stack[sp++] = num;
            ep++;
        } else if (expr[ep] == '/') {
            tmpnum = stack[--sp];
            num = stack[--sp] / tmpnum;
            stack[sp++] = num;
            ep++;
        }
    }
    printf("%7.2lf\n", stack[sp - 1]);
    return 0;
}