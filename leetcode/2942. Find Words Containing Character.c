/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
bool find_char(char* word, char x) {
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == x) return true;
    }
    return false;
}

int* findWordsContaining(char** words, int wordsSize, char x, int* returnSize) {
    *returnSize = 0;
    int *idx = (int *) malloc(sizeof(int) * wordsSize);
    for (int i = 0; i < wordsSize; i++) {
        if (find_char(words[i], x)) {
            idx[(*returnSize)++] = i;
        }
    }
    int *ans = (int *) malloc(sizeof(int) * *returnSize);
    for (int i = 0; i < *returnSize; ans[i] = idx[i], i++);
    free(idx);
    return ans;
}
