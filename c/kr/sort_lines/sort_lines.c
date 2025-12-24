#include <stdio.h>
#include "sort_lines.h"


int main(void) {
    void print_delimiter(void);
    char *arr[MAX_LINES];
    long unsigned int num_lines = 0;
    num_lines = get_lines(arr, MAX_LINES);
    print_delimiter();
    print_lines(arr, num_lines);
    print_delimiter();
    qsort(arr, 0, num_lines);
    print_lines(arr, num_lines);
    return 0;
}


void print_delimiter(void) {
    int i;
    for (i = 0; i < 80; printf("="), i++);
    printf("\n");
}
