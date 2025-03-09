/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* closestPrimes(int left, int right, int* returnSize) {
    int *result, *sieve, *prime, num_prime = 0, min_diff = right - left + 1, diff;
    result = (int *) malloc(2 * sizeof(int));
    sieve = (int *) malloc((right + 1) * sizeof(int));
    prime = (int *) malloc((right + 1) * sizeof(int));
    result[0] = result[1] = -1;
    for (int i = 0; i <= right; sieve[i] = 1, i++);
    sieve[0] = sieve[1] = 0;
    for (int i = 2; i <= right; i++) {
        if (sieve[i]) {
            for (unsigned long j = ((unsigned long) i * (unsigned long) i); j <= right; sieve[j] = 0, j += i); /* very bad :( */
        }
    }
    *returnSize = 2;
    for (int i = left; i <= right; i++) {
        if (sieve[i]) prime[num_prime++] = i;
    }
    if (num_prime < 2) return result;
    for (int i = 1; i < num_prime; i++) {
        diff = prime[i] - prime[i - 1];
        if (diff < min_diff) {
            min_diff = diff;
            result[0] = prime[i - 1];
            result[1] = prime[i];
        }
    }
 
    return result;
}
