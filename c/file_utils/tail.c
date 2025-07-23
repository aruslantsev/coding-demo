#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_SIZE 1000


int get_line(char *s, int *size) {
    int c, p;  // c: current char, p: position in string
    for (p = 0; (c = getchar()) != '\n' && c != EOF; s[p++] = c);
    s[p] = '\0';
    *size = p + 1;
    if (c == EOF) return 1; // last line in input
    return 0;
}


void get_lines(char lines[][LINE_SIZE], int lines_size, int *start_position, int *lines_to_print) {
    int curr_line = 0, c, size;
    char s[LINE_SIZE];
    while (get_line(s, &size) != 1) strcpy((lines[curr_line++ % lines_size]), s);
    if (size > 1) strcpy((lines[curr_line++ % lines_size]), s);  // copy last line only if not empty
    *start_position = lines_size - curr_line >= 0? 0: curr_line % lines_size;
    *lines_to_print = curr_line >= lines_size? lines_size: curr_line;
}


void print_lines(char lines[][LINE_SIZE], int lines_size, int start_position, int lines_to_print) {
    for (int i = 0; i < lines_to_print; i++) {
        printf("%s\n", lines[(i + start_position) % lines_size]);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s NUM_LINES\n", argv[0]);
        exit(1);
    }
    int num_lines = atoi(argv[1]), start_position, lines_to_print;
    char lines[num_lines][LINE_SIZE];
    get_lines(lines, num_lines, &start_position, &lines_to_print);
    print_lines(lines, num_lines, start_position, lines_to_print);
    return 0;
}
