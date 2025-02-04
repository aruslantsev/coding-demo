bool isArraySpecial(int* nums, int numsSize) {
    int curr, prev;
    prev = nums[0] % 2;
    for (int i = 1; i < numsSize; i++) {
        curr = nums[i] % 2;
        if (curr == prev) return false;
        prev = curr;
    }
    return true;
}
