#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "sys/file.h"


int main(int argc, char *argv[]) {
    int fd, n;
    char buf[BUFSIZ];
    for (int i = 1; i < argc; i++) {
        if ((fd = open(argv[i], O_RDONLY, 0)) == -1) {
            fprintf(stderr, "Error while opening %s\n", argv[i]);
            exit(1);
        }
        while ((n = read(fd, buf, BUFSIZ)) > 0)
            write(1, buf, n);
        close(fd);
    }
    return 0;
}
