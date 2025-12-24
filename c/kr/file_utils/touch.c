#include <stdio.h>
#include "sys/file.h"
#include "unistd.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s filename\n", argv[0]);
    }
    int fd = open(argv[1], O_RDONLY, 0);
    if (fd != -1) {
        close(fd);
    } else {
        fd = creat(argv[1], 0666);
        close(fd);
    }
    return 0;
}
