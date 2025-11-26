#include <stdio.h>
#include "solve.c"
#include "generate.c"
#include "show.c"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 15
#define COLS 80

int main(void) {
    char maze[ROWS][COLS];
    char start_x, start_y, x, y;
    enum directions dir;
    enum statuses status;

    srand(time(NULL));

    puts("Generating maze");
    status = generate_maze_by_cursor(ROWS, COLS, maze, &start_x, &start_y, &dir);
    x = start_x;
    y = start_y;
    puts("Done");

    show_maze(ROWS, COLS, maze);
    if (status != SUCCESS) {
        puts("Wrong maze");
        return 0;
    }
    printf("Start position: %d %d\n", x, y);

    do {
        status = maze_step(ROWS, COLS, maze, &x, &y, &dir);
        show_maze(ROWS, COLS, maze);
        usleep(100000);
    } while (status != SUCCESS);
    if (x == start_x && y == start_y) {
        puts("Exit not found. Returned to start position");
    } else {
        printf("Found exit: x = %d, y = %d\n", x, y);
    }
    return 0;
}
