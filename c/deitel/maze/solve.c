#include <stdio.h>
#define NUM_DIRECTIONS 4

#ifndef MAZE_SOLVE
#define MAZE_SOLVE

enum directions {UP = 0, RIGHT, DOWN, LEFT};

const int diffs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
const char cursors[5] = "^>V<X";

enum statuses {SUCCESS = 0, PROGRESS};

enum statuses maze_step(int rows, int cols, char maze[rows][cols], char *x, char *y, enum directions *direction) {
    if (maze[*x][*y] == '#') {
        puts("Error!");
        return SUCCESS;
    }
    enum statuses status = PROGRESS;
    int prev_x = *x - diffs[*direction][0];
    int prev_y = *y - diffs[*direction][1];
    if (0 <= prev_x && prev_x < rows && 0 <= prev_y && prev_y < cols) {
        maze[prev_x][prev_y] = cursors[4];
        if (*x == 0 || *x == rows - 1 || *y == 0 || *y == cols - 1)
            status = SUCCESS;
    }

    maze[*x][*y] = cursors[*direction];
    if (status == SUCCESS)
        return status;

    if (maze[*x + diffs[(*direction + 1) % NUM_DIRECTIONS][0]][*y + diffs[(*direction + 1) % NUM_DIRECTIONS][1]] != '#') {
        *direction = (*direction + 1) % NUM_DIRECTIONS;
    }
    while (maze[*x + diffs[*direction][0]][*y + diffs[*direction][1]] == '#') {
        *direction = (*direction - 1) % NUM_DIRECTIONS;
    }

    *x += diffs[*direction][0];
    *y += diffs[*direction][1];
    return status;
}

#endif
