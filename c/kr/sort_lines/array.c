#include <stdio.h>
#include "sort_lines.h"


long unsigned int get_lines(char *arr[], long unsigned int max_size) {
    long unsigned int num_lines = 0, size;
    char line[MAX_LINESIZE], *p;
    while (num_lines < max_size && (size = get_line(line)) > 1 && (p = alloc(size)) != 0) {
        strcopy(line, p);
        arr[num_lines++] = p;
    }
    return num_lines;
}


void print_lines(char *arr[], long unsigned int num_lines) {
    while (num_lines > 0) {
        printf("%s\n", *arr++);
        num_lines--;
    }
    return;
}
