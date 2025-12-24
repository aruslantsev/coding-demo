#include <unistd.h>

#define BSIZE   1024
#define EOF     -1


int getchr_unbuffered(void) {
    char c;
    return (read(0, &c, 1) == 1)? c: EOF;
}

int getchr(void) {
    static char buf[BSIZE];
    static char *bp;
    static int n = 0;

    if (n == 0) {
        n = read(0, buf, sizeof buf);
        bp = buf;
    }
    return (--n >= 0) ? (unsigned char) *bp++ : EOF;
}


int putchr(char c) {
    return write(1, &c, 1);
}
