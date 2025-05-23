/**
 * Note: The returned array must be malloced, assume caller calls free().
 */


bool compare(char *w1, char *w2) {
    if (strlen(w1) != strlen(w2)) return false;
    int diff = 0;
    for (int i = 0; w1[i] != '\0'; i++) {
        if (w1[i] != w2[i]) diff++;
        if (diff > 1) return false;
    }
    return (diff == 1);
}


char** getWordsInLongestSubsequence(char** words, int wordsSize, int* groups, int groupsSize, int* returnSize) {
    int *dp = (int *) malloc(sizeof(int) * wordsSize);
    int *prev = (int *) malloc(sizeof(int) * wordsSize);
    for (int i = 0; i < wordsSize; i++) {
        dp[i] = 1;
        prev[i] = -1;
    }
    int maxidx = 0;
    for (int i = 1; i < wordsSize; i++) {
        for (int j = 0; j < i; j++) {
            if (compare(words[i], words[j]) && groups[i] != groups[j] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                prev[i] = j;
            }
        }
        if (dp[i] > dp[maxidx]) maxidx = i;
    }

    *returnSize = dp[maxidx];
    int pos = maxidx;
    char **ans = (char **) malloc(sizeof(char *) * *returnSize);
    for (int i = *returnSize - 1; i >= 0; i--) {
        ans[i] = words[pos];
        pos = prev[pos];
    }
    return ans;
}
