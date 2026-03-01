#include <errno.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "snake.h"
#include "terminal.h"

/* 
    TODOLIST 
    1. Avoid direct write() calls. Write data to the buffer and flush buffer when refreshing the screen
    2. Use variables or constants for border size, field size, etc. 
    3. Check for screen size. If too low, don't start the game
    4. Draw banner when paused, after start and after game end.
    5. Ask when exiting
*/

#define INIT_LENGTH         3
#define INIT_LIFES          3
#define MOVE_DELAY_MS       512
#define THRESHOLD_FOOD      10
#define SIDEBAR_WIDTH       10
#define BORDER_THICKNESS    1

struct screenConfig config;

struct gameData game;


void processKeypress(void) {
    int nread;
    char c;
    if ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    if (c == '\x1b') {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 2) == 2) {
            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': 
                        if (game.currentDir != DOWN) game.currentDir = UP;
                        break;
                    case 'B': 
                        if (game.currentDir != UP) game.currentDir = DOWN;
                        break;
                    case 'D': 
                        if (game.currentDir != RIGHT) game.currentDir = LEFT;
                        break;
                    case 'C': 
                        if (game.currentDir != LEFT) game.currentDir = RIGHT;
                        break;
                }
                game.moving = true;
            }
        }
    } else {
        if (c == 'q') {
            clearScreen();
            showCursor();
            destroyGame(&game);
            exit(EXIT_SUCCESS);
        }; 
        if (c == 'p') {
            game.moving = false;
        }
    }
}


void drawBorder() {
    write(STDOUT_FILENO, "\x1b[H", 3);
    for (size_t row = 0; row < config.screenrows; row++) {
        write(STDOUT_FILENO, (row == 0 || row == config.screenrows - 1) ? "+" : "|", 1);
        for (size_t col = 0; col < config.screencols - 2 - SIDEBAR_WIDTH; col++) {
            if (row == 0 || row == config.screenrows - 1) write(STDOUT_FILENO, "-", 1);
            else write(STDOUT_FILENO, "\x1b[C", 3);
        }
        write(STDOUT_FILENO, (row == 0 || row == config.screenrows - 1) ? "+" : "|", 1);
        if (row < config.screenrows - 1) write(STDOUT_FILENO, "\r\n", 2);
    }
}


void drawSideBar() {
    char buf[SIDEBAR_WIDTH];

    write(STDOUT_FILENO, "\x1b[H", 3);
    for (size_t row = 0; row < 2; row++) write(STDOUT_FILENO, "\x1b[B", 3);
    for (size_t col = 0; col < config.screencols - SIDEBAR_WIDTH; col++) write(STDOUT_FILENO, "\x1b[C", 3);
    sprintf(buf, " Lifes: %d", game.numLifes);
    write(STDOUT_FILENO, buf, strlen(buf));

    write(STDOUT_FILENO, "\x1b[H", 3);
    for (size_t row = 0; row < 5; row++) write(STDOUT_FILENO, "\x1b[B", 3);
    for (size_t col = 0; col < config.screencols - SIDEBAR_WIDTH; col++) write(STDOUT_FILENO, "\x1b[C", 3);
    sprintf(buf, " Score: %d", game.snakeLength - INIT_LENGTH);
    write(STDOUT_FILENO, buf, strlen(buf));
}


void drawSnake() {
    for (size_t snakePtr = 0; snakePtr < game.snakeLength; snakePtr++) {
        write(STDOUT_FILENO, "\x1b[H", 3);
        write(STDOUT_FILENO, "\x1b[B", 3);
        write(STDOUT_FILENO, "\x1b[C", 3);
        for (size_t x = 0; x < game.snake[snakePtr].x; x++) write(STDOUT_FILENO, "\x1b[B", 3);
        for (size_t y = 0; y < game.snake[snakePtr].y; y++) write(STDOUT_FILENO, "\x1b[C", 3);
        write(STDOUT_FILENO, "0", 1);
    }
    write(STDOUT_FILENO, "\x1b[H", 3);
    write(STDOUT_FILENO, "\x1b[B", 3);
    write(STDOUT_FILENO, "\x1b[C", 3);
    for (size_t x = 0; x < game.foodPosition.x; x++) write(STDOUT_FILENO, "\x1b[B", 3);
    for (size_t y = 0; y < game.foodPosition.y; y++) write(STDOUT_FILENO, "\x1b[C", 3);
    write(STDOUT_FILENO, "X", 1);
    write(STDOUT_FILENO, "\x1b[H", 3);
}


void refreshScreen() {
    clearScreen();
    drawBorder();
    drawSnake();
    drawSideBar();
    write(STDOUT_FILENO, "\x1b[H", 3);
}


int main() {
    enableRawMode();
    if (getWindowSize(&config.screenrows, &config.screencols) == -1) die("getWindowSize");
    initGame(
        &game, 
        config.screenrows - (2 * BORDER_THICKNESS), 
        config.screencols - (2 * BORDER_THICKNESS) - SIDEBAR_WIDTH, 
        INIT_LENGTH, 
        INIT_LIFES
    );
    hideCursor();
    refreshScreen();

    int targetLength = INIT_LENGTH + THRESHOLD_FOOD;

    struct timespec start, end;
    long long time_diff_ms;
    long long num_moves = 0;
    int move_delay = MOVE_DELAY_MS;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (game.numLifes > 0) {
        processKeypress();
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_diff_ms = (end.tv_sec - start.tv_sec) * 1000LL + (end.tv_nsec - start.tv_nsec) / 1000000LL;
        if (time_diff_ms > move_delay) {
            if (game.moving) {
                moveSnake(&game);
                num_moves++;
                if (game.snakeLength >= targetLength) {
                    move_delay /= 2;
                    targetLength += THRESHOLD_FOOD;
                }
            }
            refreshScreen();
            clock_gettime(CLOCK_MONOTONIC, &start);
        }
    }
    clearScreen();
    showCursor();
    destroyGame(&game);
    return 0;
}
