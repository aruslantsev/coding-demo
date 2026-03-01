#pragma once

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>


struct screenConfig {
    int screenrows;
    int screencols;
    struct termios orig_termios;
};


void enableRawMode(void);

void die(const char *);

void clearScreen(void);

void hideCursor(void);

void showCursor(void);

int getWindowSize(int *, int *);
