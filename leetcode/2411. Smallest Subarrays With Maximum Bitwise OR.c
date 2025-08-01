/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* smallestSubarrays(int* nums, int numsSize, int* returnSize) {
    int *ans = (int *) malloc(numsSize * sizeof(int));
    int idx[31] = {-1};
    int nxt;
    *returnSize = numsSize;
    for(int i = numsSize - 1; i >= 0; i--) {
        nxt = i;
        for (int j = 0; j < 31; j++) {
            if (nums[i] & (1 << j)) {
                idx[j] = i;
            } else {
                nxt = nxt >= idx[j]? nxt: idx[j];
            }
        }
        ans[i] = nxt - i + 1;
    }
    return ans;
}
