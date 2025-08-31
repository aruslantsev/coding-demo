bool backtrack(char **board, int rows[], int cols[], int sqrs[]) {
    int candidates;
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (board[r][c] == '.') {
                candidates = rows[r] | cols[c] | sqrs[(r / 3) * 3 + c / 3];
                for (int cand = 0; cand < 9; cand++)
                    if (~candidates & (1 << cand)) {
                        rows[r] |= (1 << cand);
                        cols[c] |= (1 << cand);
                        sqrs[(r / 3) * 3 + c / 3] |= (1 << cand);
                        board[r][c] = '1' + cand;
                        if (backtrack(board, rows, cols, sqrs)) return true;
                        rows[r] &= ~(1 << cand);
                        cols[c] &= ~(1 << cand);
                        sqrs[(r / 3) * 3 + c / 3] &= ~(1 << cand);
                        board[r][c] = '.';
                    }
                return false;
            }
    return true;
}

void solveSudoku(char** board, int boardSize, int* boardColSize) {
    int rows[9], cols[9], sqrs[9], full_set, curr;
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (board[r][c] != '.') {
                curr = board[r][c] - '1';
                rows[r] |= (1 << curr);
                cols[c] |= (1 << curr);
                sqrs[(r / 3) * 3 + c / 3] |= (1 << curr);
            }
    backtrack(board, rows, cols, sqrs);
}
