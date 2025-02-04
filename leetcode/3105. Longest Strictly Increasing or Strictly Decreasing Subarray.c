int longestMonotonicSubarray(int* nums, int numsSize) {
    int ascendingStrike, descendingStrike, ans;
    ans = ascendingStrike = descendingStrike = 1;
    for (int i = 1; i < numsSize; i++) {
        if (nums[i] > nums[i - 1]) {
            ascendingStrike++;
            descendingStrike = 1;
        } else if (nums[i] < nums[i - 1]) {
            ascendingStrike = 1;
            descendingStrike++;
        } else {
            ascendingStrike = 1;
            descendingStrike = 1;
        }
        ans = ans > ascendingStrike ? ans : ascendingStrike;
        ans = ans > descendingStrike ? ans : descendingStrike;
    }
    return ans;
}
