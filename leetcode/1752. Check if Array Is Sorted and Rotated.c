bool check(int* nums, int numsSize) {
    int x = 0;
    if (nums[numsSize - 1] > nums[0])
        x++;
    for (int i = 1; i < numsSize; i++) {
        if (nums[i] < nums[i - 1]) {
            x++;
            if (x > 1) return false;
        }
    }
    return true;
}
