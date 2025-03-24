#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    FILE *fp;
    int c, prev = '\n', numbered = 0, firstarg = 1, line_number;
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s filename [filename ...]\n", argv[0]);
    }
    if (strcmp(argv[1], "-n") == 0) {
        numbered = 1;
        firstarg++;
    }
    for (int i = firstarg; i < argc; i++) {
        line_number = 0;
        if ((fp = fopen(argv[i], "r")) == NULL) {
            fprintf(stderr, "Error while opening %s\n", argv[i]);
            exit(1);
        }
        while ((c = getc(fp)) != EOF) {
            if (numbered && prev == '\n') {
                printf("%5d ", line_number++);
            }
            prev = c;
            putchar(c);
        }
        fclose(fp);
    }
    return 0;
}
