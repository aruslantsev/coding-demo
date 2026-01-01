/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* plusOne(int* digits, int digitsSize, int* returnSize) {
    int *ans = (int *) malloc(sizeof(int) * (digitsSize + 1));
    int addition = 1;
    for (int i = 0, j = digitsSize - 1; i < digitsSize; i++, j--) {
        ans[i] = digits[j] + addition;
        addition = ans[i] / 10;
        ans[i] %= 10;
    }
    *returnSize = digitsSize + (addition > 0);
    if (addition > 0) ans[*returnSize - 1] = addition;
    int t;
    for (int i = 0, j = *returnSize - 1; i < *returnSize / 2; i++, j--) {
        t = ans[i];
        ans[i] = ans[j];
        ans[j] = t;
    }
    return ans;
}
