int numSubmat(int** mat, int matSize, int* matColSize) {
    int *curr_row, ans = 0, curr;
    curr_row = (int *) malloc(sizeof(int) * *matColSize);
    for (int i = 0; i < *matColSize; curr_row[i++] = 0);

    for (int row = 0; row < matSize; row++) {
        for (int col = 0; col < *matColSize; col++) {
            curr_row[col] = mat[row][col] == 0? 0: curr_row[col] + 1;
            curr = curr_row[col];
            for (int tmp_col = col; tmp_col >= 0; tmp_col--) {
                curr = curr <= curr_row[tmp_col]? curr: curr_row[tmp_col];
                if (curr == 0) break;
                ans += curr;
            }
        }
    }
    free(curr_row);
    return ans;
}
