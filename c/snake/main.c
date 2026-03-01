#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#include "snake.h"
#include "terminal.h"
#include "screen.h"

/* Minimal screen size */
#define MIN_SCREEN_ROWS     20
#define MIN_SCREEN_COLS     40

/* Interface parameters */
#define SIDEBAR_WIDTH       13
#define BORDER_THICKNESS    2

/* Initial game parameters */
#define MOVE_DELAY_MS       512
#define INIT_LENGTH         3
#define INIT_LIFES          3
#define THRESHOLD_FOOD      10
#define SPEEDUP_FACTOR      0.8


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
                game.keyPressed = true;
                game.moving = true;
            }
        } else {
            clearScreen();
            destroyGame(&game);
            printf("Your final score: %d\r\n", game.score);
            exit(EXIT_SUCCESS);
        }
    } else {
        if (c == 'q') {
            clearScreen();
            destroyGame(&game);
            printf("Your final score: %d\r\n", game.score);
            exit(EXIT_SUCCESS);
        }; 
        if (c == 'p') {
            game.moving = false;
        }
    }
}


void termination_handler(int signum) {
    clearScreen();
    destroyGame(&game);
    printf("Your final score: %d\r\n", game.score);
    exit(EXIT_SUCCESS);
}


int main() {
    struct outputBuffer obuf = OBUF_INIT;

    enableRawMode();
    /* Restore screen config after recieving sigint */
    if (signal(SIGINT, termination_handler) == SIG_ERR) {
        printf("%s\r\n", "Can't catch SIGINT");
        exit(EXIT_FAILURE);
    }

    /* Check for terminal size */
    if (getWindowSize(&config.screenrows, &config.screencols) == -1) die("getWindowSize");
    config.borderThickness = BORDER_THICKNESS;
    config.sidebarSize = SIDEBAR_WIDTH;
    if (config.screencols < MIN_SCREEN_COLS || config.screenrows < MIN_SCREEN_ROWS) {
        printf(
            "Resolution: %ldx%ld symbols. Required: %dx%d symbols\r\n", 
            config.screencols, 
            config.screenrows, 
            MIN_SCREEN_COLS, 
            MIN_SCREEN_ROWS
        );
        exit(EXIT_FAILURE);
    }

    /* Initialize game */
    initGame(
        &game, 
        config.screenrows - (2 * BORDER_THICKNESS), 
        config.screencols - (2 * BORDER_THICKNESS) - SIDEBAR_WIDTH, 
        INIT_LENGTH, 
        INIT_LIFES,
        MOVE_DELAY_MS,
        SPEEDUP_FACTOR,
        THRESHOLD_FOOD
    );

    
    refreshScreen(&obuf, &config, &game);

    struct timespec start, end;
    long long time_diff_ms;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (game.numLifes > 0) {
        processKeypress();
        clock_gettime(CLOCK_MONOTONIC, &end);
        time_diff_ms = (end.tv_sec - start.tv_sec) * 1000LL + (end.tv_nsec - start.tv_nsec) / 1000000LL;
        if (time_diff_ms > game.moveDelay || game.keyPressed) {
            if (game.moving) {
                moveSnake(&game);
            }
            game.keyPressed = false;
            refreshScreen(&obuf, &config, &game);
            start = end;
        }
    }
    clearScreen();
    destroyGame(&game);
    printf("Your final score: %d\r\n", game.score);
    return 0;
}
