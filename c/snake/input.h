#pragma once

#include "snake.h"
#include "errno.h"
#include "terminal.h"


struct userInput {
    enum direction lastDirection;
    bool pauseToggled;
    bool quitRequested;
    bool actionTriggered;
};

void processKeypress(struct userInput *);
