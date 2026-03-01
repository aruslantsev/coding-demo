#pragma once

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>


struct screenConfig {
    size_t screenrows;
    size_t screencols;
    size_t borderThickness;
    size_t sidebarSize;
    struct termios orig_termios;
};


void enableRawMode(void);

void die(const char *);

void clearScreen(void);

void hideCursor(void);

void showCursor(void);

int getWindowSize(size_t *, size_t *);
