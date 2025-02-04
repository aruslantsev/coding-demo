int maxAscendingSum(int* nums, int numsSize) {
    int ans, curr_sum;
    ans = curr_sum = nums[0];
    for (int i = 1; i < numsSize; i++) {
        if (nums[i] > nums[i - 1])
            curr_sum += nums[i];
        else
            curr_sum = nums[i];
        ans = ans > curr_sum ? ans : curr_sum;
    }
    return ans;
}
