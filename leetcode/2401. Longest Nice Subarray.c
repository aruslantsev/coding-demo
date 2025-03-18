int longestNiceSubarray(int* nums, int numsSize) {
    int l = 0, r = 0, ans = 0;
    unsigned long current_window = 0;
    for (; r < numsSize; r++) {
        while (current_window & nums[r]) {
            current_window ^= nums[l++];
        }
        current_window |= nums[r];
        ans = ans > r - l + 1? ans: r - l + 1;
    }
    return ans;
}
