#include "screen.h"


static void obAppend(struct outputBuffer *obuf, const char *s, int len) {
    if ((obuf->len + len) > obuf->bufsize) {
        char *new = realloc(obuf->buffer, obuf->bufsize + BUF_APPEND);
        if (new == NULL) exit(EXIT_FAILURE);
        obuf->bufsize += BUF_APPEND;
        obuf->buffer = new;
    }
    memcpy(&(obuf->buffer)[obuf->len], s, len);
    obuf->len += len;
}


static void obFree(struct outputBuffer *obuf) {
    obuf->len = 0;
}


static void drawBorder(struct outputBuffer *obuf, struct screenConfig *config) {
    obAppend(obuf, "\x1b[H", 3);
    for (size_t row = 0; row < config->screenrows; row++) {
        if (row >= (config->borderThickness - 1) && (row < config->screenrows - (config->borderThickness - 1))) {
            if (row == (config->borderThickness - 1) || (row == config->screenrows - 1 - (config->borderThickness - 1))) {
                for (size_t col = 0; col < config->screencols - config->sidebarSize; col++) {
                    if (col < (config->borderThickness - 1) || col > (config->screencols - config->sidebarSize - 1 - (config->borderThickness - 1))) {
                        obAppend(obuf, "\x1b[C", 3);
                    } else if (col == (config->borderThickness - 1) || col == (config->screencols - config->sidebarSize - 1 - (config->borderThickness - 1))) {
                        obAppend(obuf, "+", 1);
                    } else {
                        obAppend(obuf, "-", 1);
                    }
                }
            } else {
                for (size_t col = 0; col < config->screencols - config->sidebarSize; col++) {
                    if (col == (config->borderThickness - 1) || col == (config->screencols - config->sidebarSize - 1 - (config->borderThickness - 1))) {
                        obAppend(obuf, "|", 1);
                    } else {
                        obAppend(obuf, "\x1b[C", 3);
                    }
                }
            }
        }
        if (row < config->screenrows - 1) obAppend(obuf, "\r\n", 2);
    }
}


static void drawSideBar(struct outputBuffer *obuf, struct screenConfig *config, struct gameData *game) {
    char buf[config->sidebarSize];

    obAppend(obuf, "\x1b[H", 3);
    for (size_t row = 0; row < config->borderThickness; row++) obAppend(obuf, "\x1b[B", 3);
    for (size_t col = 0; col < config->screencols - config->sidebarSize; col++) obAppend(obuf, "\x1b[C", 3);

    if (!game->moving) {
        sprintf(buf, " %s ", "PAUSE");
    } else {
        sprintf(buf, " %s ", "");
    }
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    for (size_t row = 0; row < 2; row++) obAppend(obuf, "\x1b[B", 3);


    sprintf(buf, " Lifes: %d", game->numLifes);
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    for (size_t row = 0; row < 2; row++) obAppend(obuf, "\x1b[B", 3);

    sprintf(buf, " Score: %d", game->score);
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    for (size_t row = 0; row < 2; row++) obAppend(obuf, "\x1b[B", 3);

    sprintf(buf, " %s", "Controls:");
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    for (size_t row = 0; row < 2; row++) obAppend(obuf, "\x1b[B", 3);


    sprintf(buf, " %s", "Direction:");
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    obAppend(obuf, "\x1b[B", 3);
    sprintf(buf, " %s", " Arrows");
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    for (size_t row = 0; row < 2; row++) obAppend(obuf, "\x1b[B", 3);

    sprintf(buf, " %s", "Pause:");
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    obAppend(obuf, "\x1b[B", 3);
    sprintf(buf, " %s", " p");
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    for (size_t row = 0; row < 2; row++) obAppend(obuf, "\x1b[B", 3);

    sprintf(buf, " %s", "Exit:");
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    obAppend(obuf, "\x1b[B", 3);

    sprintf(buf, " %s", " Esc, q");
    obAppend(obuf, buf, strlen(buf));
    for (size_t col = 0; col < strlen(buf); col++) obAppend(obuf, "\x1b[D", 3);
    obAppend(obuf, "\x1b[B", 3);

    sprintf(buf, " %s", " CTRL+C");
    obAppend(obuf, buf, strlen(buf));
}


static void drawSnake(struct outputBuffer *obuf, struct screenConfig *config, struct gameData *game) {
    for (size_t snakePtr = 0; snakePtr < game->snakeLength; snakePtr++) {
        obAppend(obuf, "\x1b[H", 3);
        for (size_t border = 0; border < config->borderThickness; border++){
            obAppend(obuf, "\x1b[B", 3);
            obAppend(obuf, "\x1b[C", 3);
        }
        for (size_t x = 0; x < game->snake[snakePtr].x; x++) obAppend(obuf, "\x1b[B", 3);
        for (size_t y = 0; y < game->snake[snakePtr].y; y++) obAppend(obuf, "\x1b[C", 3);
        obAppend(obuf, "0", 1);
    }
    obAppend(obuf, "\x1b[H", 3);
    for (size_t border = 0; border < config->borderThickness; border++){
        obAppend(obuf, "\x1b[B", 3);
        obAppend(obuf, "\x1b[C", 3);
    }
    for (size_t x = 0; x < game->foodPosition.x; x++) obAppend(obuf, "\x1b[B", 3);
    for (size_t y = 0; y < game->foodPosition.y; y++) obAppend(obuf, "\x1b[C", 3);
    obAppend(obuf, "X", 1);
    obAppend(obuf, "\x1b[H", 3);
}


void refreshScreen(struct outputBuffer *obuf, struct screenConfig *config, struct gameData *game) {
    clearScreen();
    drawBorder(obuf, config);
    drawSnake(obuf, config, game);
    drawSideBar(obuf, config, game);
    obAppend(obuf, "\x1b[H", 3);
    if (obuf->len > 0) {
        write(STDOUT_FILENO, obuf->buffer, obuf->len);
        obFree(obuf);
    }
}
