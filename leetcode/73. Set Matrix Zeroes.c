void setZeroes(int** matrix, int matrixSize, int* matrixColSize) {
    int cols[200];
    bool zero_row;
    for (int i = 0; i < 200; cols[i] = 1, i++);

    for (int i = 0; i < matrixSize; i++) {
        zero_row = false;
        for (int j = 0; j < *matrixColSize; j++) {
            if (matrix[i][j] == 0) {
                zero_row = true;
                cols[j] = 0;
            }
        }
        if (zero_row) for (int j = 0; j < *matrixColSize; matrix[i][j] = 0, j++);
    }
    for (int j = 0; j < *matrixColSize; j++) {
        if (cols[j] == 0) for (int i = 0; i < matrixSize; matrix[i][j] = 0, i++);
    }
}
