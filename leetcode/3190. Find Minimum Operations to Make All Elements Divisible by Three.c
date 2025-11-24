int minimumOperations(int* nums, int numsSize) {
    int ans = 0;
    for (int i = 0; i < numsSize; ans += nums[i++] % 3? 1: 0);
    return ans;
}
