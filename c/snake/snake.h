#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <time.h>

#include "terminal.h"


enum direction {
    UP          = 'u', 
    DOWN        = 'd', 
    LEFT        = 'l', 
    RIGHT       = 'r'
};


struct point {
    int x;
    int y;
};


struct gameData {
    int rows;
    int cols;
    struct point *snake;
    size_t snakeLength;
    int numLifes;
    int initLength;
    enum direction currentDir;
    bool moving;
    struct point foodPosition;
};


void initGame(struct gameData *, const int, const int, const int, const int);

void destroyGame(struct gameData *game);

void placeFood(struct gameData *);

void moveSnake(struct gameData *);
