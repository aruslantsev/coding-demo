/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* applyOperations(int* nums, int numsSize, int* returnSize) {
    int *ans, p = 0;
    *returnSize = numsSize;
    ans = (int *) malloc(numsSize * sizeof(int));
    for (int i = 0; i < numsSize - 1; i++) {
        if ((nums[i] == nums[i + 1])) {
            nums[i] *= 2;
            nums[i + 1] = 0;
        }
        if (nums[i] != 0) ans[p++] = nums[i];
    }
    ans[p++] = nums[numsSize - 1];
    for(; p < numsSize; p++)
        ans[p] = 0;
    return ans;
}
