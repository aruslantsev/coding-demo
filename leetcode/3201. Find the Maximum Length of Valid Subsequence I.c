int maximumLength(int* nums, int numsSize) {
    int ans = 0, res;
    const int pattern[4][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    for (int pn = 0; pn < 4; pn++) {
        res = 0;
        for (int nn = 0; nn < numsSize; nn++)
            if (nums[nn] % 2 == pattern[pn][res % 2]) res++;
        ans = ans > res? ans: res;
    }
    return ans;
}
