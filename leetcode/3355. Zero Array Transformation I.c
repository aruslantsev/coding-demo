bool isZeroArray(int* nums, int numsSize, int** queries, int queriesSize, int* queriesColSize) {
    int *diffs;
    diffs = (int *) malloc(sizeof(int) * (numsSize + 1));
    for (int i = 0; i < numsSize; diffs[i] = 0, i++);
    for (int i = 0; i < queriesSize; i++) {
        diffs[queries[i][0]]--;
        diffs[queries[i][1] + 1]++;
    }
    long curr = 0;
    for (int i = 0; i < numsSize; i++) {
        curr += diffs[i];
        if (nums[i] + curr > 0) return false;
    }
    return true;
}
