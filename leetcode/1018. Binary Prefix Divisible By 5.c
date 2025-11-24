/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
bool* prefixesDivBy5(int* nums, int numsSize, int* returnSize) {
    bool *ans = malloc(numsSize * sizeof(bool));
    *returnSize = numsSize;
    int num = 0;
    for (int i = 0; i < numsSize; i++) {
        num = ((num << 1) + nums[i]) % 5;
        ans[i] = num == 0;
    }
    return ans;
}
