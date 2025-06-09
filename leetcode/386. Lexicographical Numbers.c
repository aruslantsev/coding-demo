/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

void generate(int curr, int *arr, int* p, int n) {
    if (curr > n) return;
    arr[(*p)++] = curr;
    curr *= 10;
    for (int i = 0; i < 10; i++) generate(curr + i, arr, p, n);
}

int* lexicalOrder(int n, int* returnSize) {
    *returnSize = n;
    int p = 0;
    int *ans = (int *) malloc(sizeof(int) * n);
    for (int i = 1; i < 10; i++) generate(i, ans, &p, n);
    return ans;
}
