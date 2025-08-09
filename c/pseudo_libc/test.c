#include "file_support.h"

int main(void) {
    int c;
    while ((c = getchar()) != '\n') {
        putchar(c);
    }
    putchar('\n');

    FILE *fp;
    fp = fopn("test.txt", "w");
    putc('a', fp);
    putc('b', fp);
    putc('c', fp);
    putc('\n', fp);
    fcls(fp);
    fp = fopn("test.c", "r");
    while ((c = getc(fp)) != EOF) putchar(c);
    fcls(fp);
    fcls(stdout);
    fcls(stderr);
    return 0;
}
