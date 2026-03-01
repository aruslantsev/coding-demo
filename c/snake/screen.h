#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "terminal.h"
#include "snake.h"


#define OBUF_INIT {NULL, 0}

struct outputBuffer {
    char *buffer;
    int  len;
};

void refreshScreen(struct outputBuffer *, struct screenConfig *, struct gameData *);
