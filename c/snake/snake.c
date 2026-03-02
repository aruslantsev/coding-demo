#include "snake.h"


void resetGame(struct gameData *game) {
    game->currentDir = UP;
    game->snakeLength = game->initLength;
    int start_x = game->rows / 2 - game->snakeLength / 2;
    int start_y = game->cols / 2;
    for (size_t snakePtr = 0; snakePtr < game->snakeLength; snakePtr++) {
        game->snake[snakePtr] = (struct point) {.x = start_x, .y = start_y};
        start_x++;
    }
    game->moveDelay = game->initMoveDelay;
    placeFood(game);
}


void initGame(struct gameData *game, const int rows, const int cols, const int snakeLength, const int numLifes, const int moveDelay, const double speedupFactor, const int speedupLength) {
    game->snake = (struct point *) malloc(sizeof(struct point) * (rows * cols + 1));
    if (game->snake == NULL) {
        printf("%s\r\n", "Can't allocate memory");
        exit(EXIT_FAILURE);
    }
    game->snakeLength = snakeLength;
    game->initLength = snakeLength;
    game->rows = rows;
    game->cols = cols;
    game->numLifes = numLifes;
    game->score = 0;
    game->initMoveDelay = moveDelay;
    game->moveDelay = moveDelay;
    game->speedupFactor = speedupFactor;
    game->speedupLength = speedupLength;
    resetGame(game);
    srand(time(NULL));
}


void destroyGame(struct gameData *game) {
    free(game->snake);
}


void placeFood(struct gameData *game) {
    bool placed;
    struct point oldFood = game->foodPosition;
    do {
        game->foodPosition.x = rand() % game->rows;
        game->foodPosition.y = rand() % game->cols;
        if (game->foodPosition.x == oldFood.x && game->foodPosition.y == oldFood.y) continue;
        placed = true;
        for (size_t snakePtr = 0; snakePtr < game->snakeLength; snakePtr++) {
            if (
                game->foodPosition.x == game->snake[snakePtr].x 
                && game->foodPosition.y == game->snake[snakePtr].y
            ) {
                placed = false;
                break;
            }
        }
    } while (!placed);
}


enum moveResult moveSnake(struct gameData *game) {
    int new_x = game->snake[0].x;
    int new_y = game->snake[0].y;
    bool ate_food = false;
    switch (game->currentDir) {
        case UP:
            new_x--;
            break;
        case DOWN:
            new_x++;
            break;
        case LEFT:
            new_y--;
            break;
        case RIGHT:
            new_y++;
            break;
        default:
            break;
    }
    if (new_x == game->foodPosition.x && new_y == game->foodPosition.y) {
        ate_food = true;
        placeFood(game);
        game->snakeLength++;
        game->score++;
        if (game->snakeLength % game->speedupLength == 0) {
            game->moveDelay = (int) ((float) game->moveDelay * game->speedupFactor);
        }
    }
    for (size_t snakePtr = game->snakeLength - 1; snakePtr > 0; snakePtr--) {
        game->snake[snakePtr] = game->snake[snakePtr - 1];
    }
    game->snake[0].x = new_x;
    game->snake[0].y = new_y;
    if (new_x < 0 || new_x >= game->rows || new_y < 0 || new_y >= game->cols) {
        game->numLifes -= 1;
        resetGame(game);
        return MOVE_DEAD;
    }
    for (size_t snakePtr = 1; snakePtr < game->snakeLength; snakePtr++) {
        if (
            game->snake[0].x == game->snake[snakePtr].x 
            && game->snake[0].y == game->snake[snakePtr].y
        ) {
            game->numLifes -= 1;
            resetGame(game);
            return MOVE_DEAD;
        }
    }
    return ate_food ? MOVE_ATE : MOVE_OK;
}
