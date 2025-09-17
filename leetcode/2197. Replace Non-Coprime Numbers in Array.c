int get_gcd(int num1, int num2) {
    int rem;
    if (num2 > num1) {
        rem = num2;
        num2 = num1;
        num1 = rem;
    }
    do {
        rem = num1 % num2;
        num1 = num2;
        num2 = rem;
    } while (rem != 0);
    return num1;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* replaceNonCoprimes(int* nums, int numsSize, int* returnSize) {
    int pn = 0, pa = 0;
    int *ans = (int *) malloc(numsSize * sizeof(int));
    int num1, num2, gcd;
    while (pn < numsSize) {
        if (pa == 0) ans[pa++] = nums[pn++];
        else {
            num1 = nums[pn++];
            num2 = ans[--pa];
            gcd = get_gcd(num1, num2);
            if (gcd == 1) {
                ans[pa++] = num2;
                ans[pa++] = num1;
            } else {
                nums[--pn] = num1 / gcd * num2;
            }
        }
    }
    *returnSize = pa;
    return ans;
}
