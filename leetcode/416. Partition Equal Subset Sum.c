bool canPartition(int* nums, int numsSize) {
    int target = 0;
    for (int i = 0; i < numsSize; i++) target += nums[i];
    if (target % 2 != 0) return false;
    target /= 2;
    bool *dp;
    dp = (bool *) malloc(sizeof(bool) * (target + 1));
    for (int i = 0; i < target + 1; dp[i++] = false);
    dp[0] = true;
    for (int i = 0; i < numsSize; i++) {
        for (int currSum = target; currSum >= nums[i]; currSum--)
            dp[currSum] = dp[currSum] || dp[currSum - nums[i]];
    }
    return dp[target];
}
