#include <stdio.h>
#include <ctype.h>
#include "calc.h"


int getop(char s[]) {
    int i, c;
    static char buffer;  /* initialized once, saves state between calls */
    static short int buf_used = 0;

    while ((s[0] = c = (buf_used? buf_used = 0, buffer: getchar())) == ' ' || c == '\t');
    s[1] = '\0';
    if (!isdigit(c) && c != '.')
        return s[0];
    i = 0;
    if (isdigit(c))
        while (isdigit(s[++i] = c = (buf_used? buf_used = 0, buffer: getchar())));
    if (c == '.')
        while (isdigit(s[++i] = c = (buf_used? buf_used = 0, buffer: getchar())));
    s[i] = '\0';
    if (c != EOF) {
        buffer = c;
        buf_used = 1;
    }
    return NUMBER;
}
