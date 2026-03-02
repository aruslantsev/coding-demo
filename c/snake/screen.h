#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "terminal.h"
#include "snake.h"
#include "input.h"


#define OBUF_INIT {NULL, 0, 0}
#define BUF_APPEND  1024

struct outputBuffer {
    char *buffer;
    int  bufsize;
    int  len;
};

void refreshScreen(struct outputBuffer *, struct screenConfig *, struct gameData *, struct userInput *);
