long long solve(int** questions, int questionsSize, int idx, long long* dp) {
    if (idx >= questionsSize) return 0;
    long long skip, ans;
    if (dp[idx] < 0) {
        skip = questions[idx][0] + solve(questions, questionsSize, idx + questions[idx][1] + 1, dp);
        ans = solve(questions, questionsSize, idx + 1, dp);
        dp[idx] = skip > ans? skip: ans;
    }
    return dp[idx];
}

long long mostPoints(int** questions, int questionsSize, int* questionsColSize) {
    long long *dp, ans;
    dp = (long long *) malloc(sizeof(long long) * questionsSize);
    for (int i = 0; i < questionsSize; i++) dp[i] = -1;
    solve(questions, questionsSize, 0, dp);
    ans = dp[0];
    free(dp);
    return ans;
}
