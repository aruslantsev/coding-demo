int find(int num, int *arr, int arrSize) {
    int lo = 0, hi = arrSize - 1, mid;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (arr[mid] < num) lo = mid + 1;
        else if (arr[mid] > num) hi = mid - 1;
        else return mid;
    }
    return -1;
}

int fibo(int i, int j, int dp[][1000], int *arr, int arrSize) {
    if (dp[i][j] == -1) {
        int pos, nxt;
        nxt = arr[i] + arr[j];
        if ((pos = find(nxt, arr, arrSize)) != -1) {
            dp[i][j] = 1 + fibo(j, pos, dp, arr, arrSize);
        } else {
            dp[i][j] = 0;
        }
    }
    return dp[i][j];
}

int lenLongestFibSubseq(int* arr, int arrSize) {
    int dp[1000][1000];
    int ans = 0, tmp;
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)
            dp[i][j] = -1;
    for (int i = 0; i < arrSize - 1; i++)
        for (int j = i + 1; j < arrSize; j++)
            {
                tmp = 2 + fibo(i, j, dp, arr, arrSize);
                ans = ans > tmp? ans: tmp;
            }
    if (ans == 2) return 0;
    return ans;
}


/* slower, uses more ram */

int find(int num, int *arr, int arrSize) {
    int lo = 0, hi = arrSize - 1, mid;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        if (arr[mid] < num) lo = mid + 1;
        else if (arr[mid] > num) hi = mid - 1;
        else return mid;
    }
    return -1;
}

int fibo(int i, int j, int **dp, int *arr, int arrSize) {
    if (dp[i][j] == -1) {
        int pos;
        int nxt = arr[i] + arr[j];
        if ((pos = find(nxt, arr, arrSize)) != -1) {
            dp[i][j] = 1 + fibo(j, pos, dp, arr, arrSize);
        } else {
            dp[i][j] = 0;
        }
    }
    return dp[i][j];
}

int lenLongestFibSubseq(int* arr, int arrSize) {
    int* dp[arrSize];
    int ans = 0, tmp;
    for (int i = 0; i < arrSize; i++) {
        dp[i] = (int *) malloc(arrSize * sizeof(int));
        for (int j = 0; j < arrSize; j++)
            dp[i][j] = -1;
    }
    for (int i = 0; i < arrSize - 1; i++)
        for (int j = i + 1; j < arrSize; j++)
            {
                tmp = 2 + fibo(i, j, dp, arr, arrSize);
                ans = ans > tmp? ans: tmp;
            }
    if (ans == 2) return 0;
    return ans;
}
