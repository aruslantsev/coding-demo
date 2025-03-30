typedef struct node *stackptr;
typedef struct node {
    int val; 
    stackptr prev;
} stack_elem;
typedef struct pair {
    int val; 
    int idx;
} elem_info;

int compare_elems(const void* a, const void* b)
{
    elem_info arg1 = *(const elem_info *) a;
    elem_info arg2 = *(const elem_info *) b;
    if (arg1.val < arg2.val) return 1;
    if (arg1.val > arg2.val) return -1;
    if (arg1.idx < arg2.idx) return -1;
    if (arg1.idx > arg2.idx) return 1;
    return 0;
}

int *getArray(int arrSize, int val) {
    int *arr;
    arr = (int *) malloc(arrSize * sizeof(int));
    for (int i = 0; i < arrSize; arr[i] = val, i++);
    return arr;
}

int primeScore(int num) {
    int score = 0;
    for (int factor = 2; factor < (int) sqrt(num) + 1; factor++) {
        if (num % factor == 0) {
            score++;
            while (num % factor == 0) num /= factor;
            }
    }
    if (num > 1) score++;
    return score;
}

int maximumScore(int* nums, int numsSize, int k) {
    long long score = 1, mod = 1000000007;;
    elem_info elems[numsSize];
    int *prime_scores, *prev_dominant = getArray(numsSize, -1), *next_dominant = getArray(numsSize, numsSize);
    long long *num_subarrays;
    prime_scores = (int *) malloc(numsSize * sizeof(int));
    num_subarrays = (long long *) malloc(numsSize * sizeof(long long));

    for (int i = 0; i < numsSize; i++) prime_scores[i] = primeScore(nums[i]);

    stack_elem *stack, *curr;
    stack = (stack_elem *) malloc(sizeof(stack_elem));
    stack -> val = -1; 
    stack -> prev = NULL;
    for (int i = 0; i < numsSize; i++) {
        while ((stack -> val >= 0) && (prime_scores[stack -> val] < prime_scores[i])) {
            next_dominant[stack -> val] = i;
            curr = stack;
            stack = stack -> prev;
            free(curr);
        }
        if (stack -> val >= 0) {
            prev_dominant[i] = stack -> val;
        }
        curr = (stack_elem *) malloc(sizeof(stack_elem));
        curr -> val = i;
        curr -> prev = stack;
        stack = curr;
    }
    while (stack -> val >= 0) {
        curr = stack;
        stack = stack -> prev;
        free(curr);
    }
    free(stack);

    for (int i = 0; i < numsSize; i++) {
        num_subarrays[i] = ((long long) next_dominant[i] - i) * (i - (long long) prev_dominant[i]);
    }

    for (int i = 0; i < numsSize; i++) {
        elems[i].val = nums[i];
        elems[i].idx = i;
    }
    qsort(elems, numsSize, sizeof(elem_info), compare_elems);
    int idx, power;
    long long num, p;
    for (int i = 0; i < numsSize; i++) {
        num = elems[i].val;
        idx = elems[i].idx;
        power = num_subarrays[idx] < k? num_subarrays[idx]: k;
        k -= power;
        p = 1;
        while (power >= 1) {
            if (power % 2 == 1) p = (p * num) % mod;
            power /= 2;
            num = (num * num) % mod;
        }
        score = (score * p) % mod;
    }
    free(prime_scores);
    free(prev_dominant);
    free(next_dominant);
    free(num_subarrays);
    return (int) score;
}
