void sortColors(int* nums, int numsSize) {
    int counts[] = {0, 0, 0};
    for (int i = 0; i < numsSize; i++) {
        counts[nums[i]]++;
    }
    int pos = 0;
    for (int color = 0; color < 3; color++) {
        for (int i = pos; i < pos + counts[color]; i++) {
            nums[i] = color;
        }
        pos += counts[color];
    }
}
