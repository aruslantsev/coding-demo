#include <stdio.h>

/*  cc -std=c99 countsym.c */

int main() {
    long cnt = 0;
    for (cnt = 0; getchar() != EOF; cnt++);
    printf("%ld", cnt);
    return 0;
}
