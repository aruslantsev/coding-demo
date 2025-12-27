int cmp(const void *a, const void *b) {
    return (*(char *)a - *(char *)b);
}


int minDeletionSize(char** strs, int strsSize) {
    char *curr, *sorted;
    curr = (char *) malloc(sizeof(char) * (strsSize + 1));
    sorted = (char *) malloc(sizeof(char) * (strsSize + 1));
    size_t ptr = 0;
    int ans = 0;

    while (strs[0][ptr] != '\0') {
        for (int row = 0; row < strsSize; row++) {
            curr[row] = strs[row][ptr];
        }
        ptr++;
        curr[strsSize] = '\0';
        strcpy(sorted, curr);
        qsort(sorted, strsSize, sizeof(char), cmp);
        if (strcmp(curr, sorted) != 0)
            ans++;
    }
    free(curr);
    free(sorted);
    return ans;
}


/**************************************************************************/

int minDeletionSize(char** strs, int strsSize) {
    int ans = 0;
    for (int j = 0; strs[0][j] != '\0'; j++)
        for (int i = 0; i < strsSize - 1; i++)
            if (strs[i][j] > strs[i + 1][j]) {
                ans++;
                break;
            }
    return ans;
}

