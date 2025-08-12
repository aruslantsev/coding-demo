#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sys/file.h"
#include "sys/stat.h"
#include "sys/dir.h"
#include <dirent.h>
#include "unistd.h"

/* in dir.h
#define NAME_MAX    14
*/

typedef struct {
    long ino;                   // inode
    char name[NAME_MAX + 1];    // name
} Dirent;

/*
#undef DIR
typedef struct {
    int fd;
    Dirent d;
} DIR;
*/


/* in file.h
struct stat {
    dev_t   st_dev;
    ino_t   st_ino;
    short   st_mode;
    short   st_nlink;
    short   st_uid;
    short   st_gid;
    dev_t   st_rdev;
    off_t   st_size;
    time_t  st_atime;
    time_t  st_mtime;
    time_t  st_ctime;
};
*/

/* in stat.h
#define S_IFMT  0160000
#define S_IFDIR 0040000
#define S_IFCHR 0020000
#define S_IFBLK 0060000
#define S_IFREG 0100000
*/

#ifndef DIRSIZ
#define DIRSIZ  14
#endif

/* in dir.h
struct direct {
    ino_t   d_ino;
    char    d_name[DIRSIZ];
};
*/


/*
DIR *opendir(const char *dirname) {
    int fd;
    struct stat stbuf;
    DIR *dp;
    if (
        (fd = open(dirname, O_RDONLY, 0)) == -1
        || fstat(fd, &stbuf) == -1
        || (stbuf.st_mode & S_IFMT) != S_IFDIR
        || (dp = (DIR *) malloc(sizeof(DIR))) == NULL
    )
        return NULL;
    dp->fd = fd;
    return dp;
}
*/
/*
void closedir(DIR *dp) {
    if (dp) {
        close(dp->fd);
        free(dp);
    }
}
*/
/*
Dirent *readdir(DIR *dp) {
    struct direct dirbuf;
    static Dirent d;
    while (read(dp->fd, (char *) &dirbuf, sizeof(dirbuf)) == sizeof(dirbuf)) {
        if (dirbuf.d_ino == 0) continue;
        d.ino = dirbuf.d_ino;
        strncpy(d.name, dirbuf.d_name, DIRSIZ);
        d.name[DIRSIZ] = '\0';
        printf("%s\n", d.name);
        return &d;
    }
    printf("Noting to read\n");
    return NULL;
}
*/

#define MAX_PATH    1024
void dirwalk(char *dir, void (*fcn)(char *)) {
    char name[MAX_PATH];
    struct dirent *dp;
    DIR *dfd;
    if ((dfd = opendir(dir)) == NULL) {
        printf("dirwalk: can't open %s\n", name);
        return;
    }
    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
        if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
            printf("dirwalk: name %s/%s too long", dir, dp->d_name);
        else {
            sprintf(name, "%s/%s", dir, dp->d_name);
            (*fcn)(name);
        }
    }
    closedir(dfd);
}

void fsize(char *name) {
    struct stat stbuf;
    if (stat(name, &stbuf) == -1) {
        printf("fsize: can't access %s\n", name);
        return;
    }
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(name, fsize);
    printf("%8ld %s\n", stbuf.st_size, name);
}


int main(int argc, char *argv[]) {
    if (argc == 1) {
        fsize(".");
    } else {
        while (--argc > 0) fsize(*++argv);
    }
    return 0;
}
