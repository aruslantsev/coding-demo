#include "input.h"


void processKeypress(struct userInput *input) {
    int nread;
    char c;
    input->actionTriggered = false;
    if ((nread = read(STDIN_FILENO, &c, 1)) == 1) {
        input->actionTriggered = true;
        if (c == '\x1b') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 2) == 2) {
                if (seq[0] == '[') {
                    switch (seq[1]) {
                        case 'A': 
                            input->lastDirection = UP;
                            break;
                        case 'B': 
                            input->lastDirection = DOWN;
                            break;
                        case 'D': 
                            input->lastDirection = LEFT;
                            break;
                        case 'C': 
                            input->lastDirection = RIGHT;
                            break;
                        default:
                            input->quitRequested = true;
                            break;
                    }
                    input->pauseToggled = false;
                } else {
                    input->quitRequested = true;
                }
            } else {
                input->quitRequested = true;
            }
        } else {
            if (c == 'q') {
                input->quitRequested = true;
            }; 
            if (c == 'p') {
                input->pauseToggled = true;
            }
        }
    } else {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    
}
