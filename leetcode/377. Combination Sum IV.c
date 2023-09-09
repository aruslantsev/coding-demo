int combinationSum4(int* nums, int numsSize, int target){
    int cache[target + 1];
    for (int i; i <= target; i++) cache[i] = -1;

    int check(int remaining) {
        if (remaining == 0) return 1;
        if (remaining < 0) return 0;

        if (cache[remaining] < 0) {
            int ans = 0;
            for (int i = 0; i < numsSize; i++) ans += check(remaining - nums[i]);
            cache[remaining] = ans;
        }
        return cache[remaining];
    }
    return check(target);
}
