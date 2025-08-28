/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

int cmp_asc(const void *a, const void *b) {return *(int *) a - *(int *) b;}

int cmp_desc(const void *a, const void *b) {return cmp_asc(b, a);}

int** sortMatrix(int** grid, int gridSize, int* gridColSize, int* returnSize, int** returnColumnSizes) {
    *returnSize = gridSize;
    int *arr = (int *) malloc(gridSize * sizeof(int));
    *returnColumnSizes = (int *) malloc(sizeof(int) * gridSize);
    for (int i = 0; i < gridSize; (*returnColumnSizes)[i++] = gridSize);

    for (int idx = 0; idx < gridSize; idx++) {
        for (int row = 0, col = idx; col < gridSize; row++, col++) arr[row] = grid[row][col];
        qsort(arr, gridSize - idx, sizeof(int), cmp_asc);
        for (int row = 0, col = idx; col < gridSize; row++, col++) grid[row][col] = arr[row];

        for (int row = idx, col = 0; row < gridSize; row++, col++) arr[col] = grid[row][col];
        qsort(arr, gridSize - idx, sizeof(int), cmp_desc);
        for (int row = idx, col = 0; row < gridSize; row++, col++) grid[row][col] = arr[col];
    }
    free(arr);
    return grid;
}
