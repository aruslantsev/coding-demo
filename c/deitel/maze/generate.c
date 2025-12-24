#include <stdlib.h>
#include "solve.c"

#define EMPTY_RATIO 0.6
#define NUM_MOVEMENTS_RATIO 0.4
#define DIR_CHANGE_RATIO 0.2


enum statuses generate_maze(char rows, char cols, char maze[rows][cols], char *start_x, char *start_y, enum directions *start_dir) {
    enum directions dir;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++)
            maze[row][col] = '#';
    }
    int row, col;

    for (int iter = 0; iter < rows * cols * EMPTY_RATIO; iter++) {
        row = rand() % (rows - 2) + 1;
        col = rand() % (cols - 2) + 1;
        maze[row][col] = '.';
    }

    /* Entrance and exit */
    for (int i = 0; i < 2; i++) {
        switch (rand() % 4) {
            case 0:
                row = 0;
                col = rand() % (cols - 2) + 1;
                break;
            case 1:
                row = rows - 1;
                col = rand() % (cols - 2) + 1;
                break;
            case 2:
                row = rand() % (rows - 2) + 1;
                col = 0;
                break;
            case 3:
                row = rand() % (rows - 2) + 1;
                col = cols - 1;
                break;
        }
        maze[row][col] = '.';

        if (row == 0)
            dir = DOWN;
        else if (row == rows - 1)
            dir = UP;
        else if (col == 0)
            dir = RIGHT;
        else
            dir = LEFT;

        /* Check if it is possible to move from this point. If it is not, don't return SUCCESS */
        if (maze[row + diffs[dir][0]][col + diffs[dir][1]] == '#') {
            return PROGRESS;
        }

        *start_x = row;
        *start_y = col;
        *start_dir = dir;
    }

    return SUCCESS;
}


enum statuses generate_maze_by_cursor(char rows, char cols, char maze[rows][cols], char *start_x, char *start_y, enum directions *start_dir) {
    enum directions dir;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++)
            maze[row][col] = '#';
    }
    int row, col, next_row, next_col;

    /* Entrance and exit */
    for (int i = 0; i < 2; i++) {
        switch (rand() % 4) {
            case 0:
                row = 0;
                col = rand() % (cols - 2) + 1;
                break;
            case 1:
                row = rows - 1;
                col = rand() % (cols - 2) + 1;
                break;
            case 2:
                row = rand() % (rows - 2) + 1;
                col = 0;
                break;
            case 3:
                row = rand() % (rows - 2) + 1;
                col = cols - 1;
                break;
        }
        maze[row][col] = '.';
        if (row == 0)
            dir = DOWN;
        else if (row == rows - 1)
            dir = UP;
        else if (col == 0)
            dir = RIGHT;
        else
            dir = LEFT;

        *start_x = row;
        *start_y = col;
        *start_dir = dir;

        next_row = row + diffs[dir][0];
        next_col = col + diffs[dir][1];
        for (int movement = 0; movement < NUM_MOVEMENTS_RATIO * rows * cols; movement++) {
            row = next_row;
            col = next_col;
            maze[row][col] = '.';
            do {
                if (rand() % 100 < DIR_CHANGE_RATIO * 100)
                    dir = rand() % 4;
                next_row = row + diffs[dir][0];
                next_col = col + diffs[dir][1];
            } while (1 > next_row || next_row >= rows - 1 || 1 > next_col || next_col >= cols - 1);
        }
    }

    return SUCCESS;
}
