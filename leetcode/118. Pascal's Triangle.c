/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** generate(int numRows, int* returnSize, int** returnColumnSizes) {
    *returnSize = numRows;
    int **ans = (int **) malloc(sizeof(int *) * numRows);
    *returnColumnSizes = (int *) malloc(sizeof(int) * numRows);
    for (int row = 0; row < numRows; row++) {
        ans[row] = (int *) malloc(sizeof(int) * (row + 1));
        (*returnColumnSizes)[row] = row + 1;
        ans[row][0] = ans[row][row] = 1;
        for (int pos = 1; pos <= row / 2; pos++) {
            ans[row][pos] = ans[row][row - pos] = ans[row - 1][pos - 1] + ans[row - 1][pos];
        }
    }
    return ans;
}
