#include <sys/file.h>
#include <unistd.h>
#include <stdlib.h>

#undef NULL
#undef getc
#undef putc

#define OPEN_MAX    16
#define BUFSIZ   4
#define NULL    0
#define EOF     (-1)


typedef struct _iobuf {
    int cnt;
    char *ptr;
    char *base;
    int flag;
    int fd;
} FILE;


enum _flags {
    _READ = 01,
    _WRITE = 02,
    _UNBUF = 04,
    _EOF = 010,  // oct 010 = dec 8
    _ERR = 020,
};

FILE _iob[OPEN_MAX] = {
    {0, NULL, NULL, _READ, 0},
    {0, NULL, NULL, _WRITE, 1},
    {0, NULL, NULL, _WRITE | _UNBUF, 2}
};

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])

#define feof(f)         (((f)->flag & _EOF) != 0)
#define ferr(f)         (((f)->flag & _ERR) != 0)
#define fileno(f)       (f)->fd

#define getc(p)         (--(p)->cnt >= 0? (unsigned char) *(p)->ptr++: _fillbuf(p))
#define putc(x, p)      (--(p)->cnt >= 0? *(p)->ptr++ = (x): _flushbuf((x), p))
#define getchar()       getc(stdin)
#define putchar(c)      putc(c, stdout)

#define print(x)        write(1, (x), sizeof(x))

int _fillbuf(FILE *fp) {
    int bufsize;
    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ) return EOF;
    bufsize = (fp->flag & _UNBUF)? 1: BUFSIZ;
    if (fp->base == NULL) {
        if ((fp->base = (char *) malloc(bufsize)) == NULL) {
            fp->flag |= _ERR;
            return EOF;
        }
    }
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1) {
            fp->flag |= _EOF;
        } else {
            fp->flag |= _ERR;
        }
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}

int _flushbuf(int c, FILE *fp) {
    int bufsize = (fp->flag & _UNBUF)? 1: BUFSIZ;
    int written;
    if ((fp->flag & (_WRITE | _EOF | _ERR)) != _WRITE) return EOF;
    if (fp->base == NULL) {
        if ((fp->base = (char *) malloc(bufsize)) == NULL) {
            fp->flag |= _ERR;
            return EOF;
        }
        fp->cnt = bufsize;
        fp->ptr = fp->base;
    }
    written = write(fp->fd, fp->base, bufsize - fp->cnt);
    if (written != bufsize - fp->cnt)
        fp->flag |= _ERR;
    fp->cnt = bufsize;
    fp->ptr = fp->base;
    if (c != NULL) {
        *fp->ptr++ = (unsigned char) c;
        fp->cnt--;
    }
};

FILE *fopn(char *name, char *mode) {
    int fd;
    FILE *fp;
    if (*mode != 'w' && *mode != 'r' && *mode != 'a') return NULL;
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if ((fp->flag & (_READ | _WRITE)) == 0) break;
    if (fp >= _iob + OPEN_MAX) return NULL;

    if (*mode == 'w') fd = creat(name, 0666);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, 0666);
        lseek(fd, 0L, 2);
    }
    else fd = open(name, O_RDONLY, 0);
    if (fd == -1) return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flag = (*mode == 'r')? _READ: _WRITE;
    return fp;
}

int fcls(FILE *fp) {
    if (fp == NULL) return -1;
    if ((fp->flag & _WRITE) != 0) _flushbuf(NULL, fp);
    free(fp->base);
    close(fp->fd);
    fp->fd = NULL;
    fp->cnt = 0;
    fp->base = fp->ptr = NULL;
    fp->flag = 0;
    return 0;
}
