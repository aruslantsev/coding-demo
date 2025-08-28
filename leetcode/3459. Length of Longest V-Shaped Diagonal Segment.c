#define max(a, b)   (a) > (b)? (a): (b)

int direction[4][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};

int search(int i, int j, int dir, bool can_turn, int next_val, int dp[][500][4][2], int** grid, int m, int n) {
    if (dp[i][j][dir][can_turn] >= 0) return dp[i][j][dir][can_turn];
    int n_i, n_j, res;
    n_i = i + direction[dir][0];
    n_j = j + direction[dir][1];
    if (n_i < 0 || n_i >= m || n_j < 0 || n_j >= n || grid[n_i][n_j] != next_val) {
        dp[i][j][dir][can_turn] = 0;
    } else {
        res = search(n_i, n_j, dir, can_turn, 2 - next_val, dp, grid, m, n);
        if (can_turn) {
            res = max(res, search(n_i, n_j, (dir + 1) % 4, false, 2 - next_val, dp, grid, m, n));
        }
        dp[i][j][dir][can_turn] = 1 + res;
    }
    return dp[i][j][dir][can_turn];
}

int lenOfVDiagonal(int** grid, int gridSize, int* gridColSize) {
    int dp[500][500][4][2];
    int m = gridSize, n = *gridColSize;
    int ans = 0;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < 4; k++)
                for (int l = 0; l < 2; l++)
                    dp[i][j][k][l] = -1;
    }
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (grid[i][j] == 1)
                for (int dir = 0; dir < 4; dir ++)
                    ans = max(ans, 1 + search(i, j, dir, true, 2, dp, grid, m, n));
    return ans;
}

/* ==================================================================================== */


#define max(a, b)   (a) > (b)? (a): (b)

int direction[4][2] = {{1, 1}, {1, -1}, {-1, -1}, {-1, 1}};
int dp[500][500][4][2];

int search(int i, int j, int dir, bool can_turn, int next_val, int** grid, int m, int n) {
    if (dp[i][j][dir][can_turn] >= 0) return dp[i][j][dir][can_turn];
    int n_i, n_j, res;
    n_i = i + direction[dir][0];
    n_j = j + direction[dir][1];
    if (n_i < 0 || n_i >= m || n_j < 0 || n_j >= n || grid[n_i][n_j] != next_val) {
        dp[i][j][dir][can_turn] = 0;
    } else {
        res = search(n_i, n_j, dir, can_turn, 2 - next_val, grid, m, n);
        if (can_turn) {
            res = max(res, search(n_i, n_j, (dir + 1) % 4, false, 2 - next_val, grid, m, n));
        }
        dp[i][j][dir][can_turn] = 1 + res;
    }
    return dp[i][j][dir][can_turn];
}

int lenOfVDiagonal(int** grid, int gridSize, int* gridColSize) {
    int m = gridSize, n = *gridColSize;
    int ans = 0;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < 4; k++)
                for (int l = 0; l < 2; l++)
                    dp[i][j][k][l] = -1;

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (grid[i][j] == 1)
                for (int dir = 0; dir < 4; dir ++)
                    ans = max(ans, 1 + search(i, j, dir, true, 2, grid, m, n));
    return ans;
}
