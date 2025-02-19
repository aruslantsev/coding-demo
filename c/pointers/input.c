#include <stdio.h>
#include "sort_lines.h"


long unsigned int get_line(char s[]) {
    long unsigned int i = 0;
    char c;
    for (i = 0; (c = getchar()) != '\n' && i < MAX_LINESIZE - 1; s[i++] = c);
    s[i] = '\0';
    return i + 1;  /* Size for \0 */
}


void strcopy(char from[], char to[]) {
    long unsigned int i = 0;
    for (i = 0; (to[i] = from[i]) != '\0'; i++);
    return;
}
