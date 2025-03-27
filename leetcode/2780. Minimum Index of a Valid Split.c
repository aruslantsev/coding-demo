int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}


int minimumIndex(int* nums, int numsSize) {
    int *sorted_nums = (int *) malloc(numsSize * sizeof(int));
    int current_num = 0, current_cnt = 0, num, cnt = 0;
    int l = 0, r = numsSize, le, re;
    for (int i = 0; i < numsSize; sorted_nums[i] = nums[i], i++);
    qsort(sorted_nums, numsSize, sizeof(int), compare_ints);
    for (int i = 0; i < numsSize; i++) {
        if (sorted_nums[i] == current_num) {
            current_cnt++;
        } else {
            current_cnt = 1;
            current_num = sorted_nums[i];
        }
        if (current_cnt > cnt) {
            cnt = current_cnt;
            num = current_num;
        }
    }
    le = 0;
    re = cnt;
    for (int i = 0; i < numsSize - 1; i++) {
        l++;
        r--;
        if (nums[i] == num) {
            le++;
            re--;
        }
        if (((float) le / l > 0.5) && ((float) re / r > 0.5)) return i;
    }
    return -1;
}
