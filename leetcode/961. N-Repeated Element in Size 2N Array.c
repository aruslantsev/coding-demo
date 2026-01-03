int repeatedNTimes(int* nums, int numsSize) {
    bool seen[10000] = {0};
    for (size_t ptr = 0; ptr < numsSize; ptr++) {
        if (seen[nums[ptr]]) return nums[ptr];
        seen[nums[ptr]] = true;
    }
    return 0;
}
