#include <stdio.h>


int add(int a, int b) { return a + b; }

int sub(int a, int b) { return a - b; }

int mul(int a, int b) { return a * b; }

int div(int a, int b) { return a / b; }

void do_op(int a, int b, int (*op)(int, int)) {
    printf("%d\n", op(a, b));
}

void do_op_1(int a, int b, int (*op)(int, int)) {
    printf("%d\n", (*op)(a, b));  /* K&R example */
}

int main(void) {
    int (*fptr)(int, int);
    int (*farr[])(int, int) = {add, sub, mul, div};

    fptr = &add;
    printf("%d\n", fptr(10, 5));

    fptr = sub;
    printf("%d\n", fptr(10, 5));

    do_op(10, 5, mul);

    do_op(10, 5, &div);

    do_op_1(10, 10, div);

    printf("%d\n", farr[3](10, 5));

    do_op(4, 6, sub(10, 5)? mul: div);

    do_op(5, 6, (int (*)(int, int))(sub(10, 5)? mul: div));  /* cast type, K&R example */

    return 0;
}
