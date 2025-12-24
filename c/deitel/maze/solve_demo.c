#include <stdio.h>
#include "solve.c"
#include "show.c"
#include <unistd.h>

#define ROWS 12
#define COLS 12

int main(void) {
    char maze[ROWS][COLS] = {
        "############",
        "#...#......#",
        "..#.#.####.#",
        "###.#....#.#",
        "#....###.#..",
        "####.#...#.#",
        "#..#.#.#.#.#",
        "##.#.#.#.#.#",
        "#......#.#.#",
        "######.###.#",
        "#......#...#",
        "############",
    };

    char start_x = 2, start_y = 0, x, y;
    x = start_x;
    y = start_y;
    enum directions dir = RIGHT;
    enum statuses status;
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
