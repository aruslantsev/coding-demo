#include <stddef.h>
#include "sort_lines.h"


static char chars[MAX_ALLOC_SIZE];
static char *pointer = chars;


char* alloc(long unsigned int size) {
    char *p;
    if (pointer + size - chars >= MAX_ALLOC_SIZE)
        return NULL;
    p = pointer;
    pointer += size;
    return p;
}
