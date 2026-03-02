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


enum moveResult {
    MOVE_OK     = 0,
    MOVE_ATE    = 1,
    MOVE_DEAD   = -1,
};


struct gameData {
    size_t rows;
    size_t cols;
    struct point *snake;
    size_t snakeLength;
    int numLifes;
    int initLength;
    enum direction currentDir;
    struct point foodPosition;
    int moveDelay;
    int score;
    int initMoveDelay;
    double speedupFactor;
    int speedupLength;
};


void initGame(struct gameData *, const int, const int, const int, const int, const int, const double, const int);

void resetGame(struct gameData *);

void destroyGame(struct gameData *game);

void placeFood(struct gameData *);

enum moveResult moveSnake(struct gameData *);
