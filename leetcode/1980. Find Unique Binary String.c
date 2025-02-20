char* findDifferentBinaryString(char** nums, int numsSize) {
    char* ans = (char *) malloc((numsSize + 1) * sizeof(char));
    for (int i = 0; i < numsSize; i++)
        ans[i] = nums[i][i] == '1' ? '0': '1';
    ans[numsSize] = '\0';
    return ans;
}
