int compare(const void *a, const void *b) {
    return *(long long *) a - *(long long *) b;

}

long long putMarbles(int* weights, int weightsSize, int k) {
    long long ans = 0, *pair_weights;
    pair_weights = (long long *) malloc(sizeof(long long) * (weightsSize - 1));
    for (int i = 0; i < weightsSize - 1; i++) pair_weights[i] = weights[i] + weights[i + 1];
    qsort(pair_weights, weightsSize - 1, sizeof(long long), compare);
    for (int i = 0; i < k - 1; i++) {
        ans += pair_weights[weightsSize - 2 - i] - pair_weights[i];
    }
    free(pair_weights);
    return ans;
}
