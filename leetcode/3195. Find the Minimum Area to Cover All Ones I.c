int minimumArea(int** grid, int gridSize, int* gridColSize) {
    int min_row = gridSize, max_row = -1, min_col = *gridColSize, max_col = -1;
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < *gridColSize; col++) {
            if (grid[row][col] != 0) {
                min_row = row < min_row? row: min_row;
                max_row = row > max_row? row: max_row;
                min_col = col < min_col? col: min_col;
                max_col = col > max_col? col: max_col;
            }
        }
    }
    if (min_row == gridSize) return 0;
    return (max_row - min_row + 1) * (max_col - min_col + 1);
}
