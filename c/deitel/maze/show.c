#include <stdio.h>

void show_maze(int rows, int cols, char maze[rows][cols]) {
    puts("");
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++)
            printf("%c", maze[row][col]);
        puts("");
    }
    puts("");
}
