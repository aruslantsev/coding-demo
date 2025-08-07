#define max(a, b) ((a) > (b) ? (a) : (b))

int bl(int *arr, int arr_len, int val) {
    int l = 0, r = arr_len - 1, m, res = arr_len;
    while (r >= l) {
        m = l + (r - l) / 2;
        if (arr[m] < val) {
            l = m + 1;
        } else {
            res = m;
            r = m - 1;
        }
    }
    return res;
}

int br(int *arr, int arr_len, int val) {
    int l = 0, r = arr_len - 1, m, res = arr_len;
    while (r >= l) {
        m = l + (r - l) / 2;
        if (arr[m] > val) {
            r = m - 1;
            res = m;
        } else {
            l = m + 1;
        }
    }
    return res;
}

int maxTotalFruits(int** fruits, int fruitsSize, int* fruitsColSize, int startPos, int k) {
    int *positions, *num_fruits, ans = 0, l_pos, r_pos, start, end;
    positions = (int *) malloc(sizeof(int) * fruitsSize);
    num_fruits = (int *) malloc(sizeof(int) * (fruitsSize + 1));
    num_fruits[0] = 0;
    for (int i = 0; i < fruitsSize; i++) {
        positions[i] = fruits[i][0];
        num_fruits[i + 1] = num_fruits[i] + fruits[i][1];
    }

    for (int steps = 0; steps < k / 2 + 1; steps++) {
        printf("%d\n", steps);
        l_pos = startPos - steps;
        r_pos = startPos + (k - 2 * steps);
        start = bl(positions, fruitsSize, l_pos);
        end = br(positions, fruitsSize, r_pos);
        ans = max(ans, num_fruits[end] - num_fruits[start]);


        r_pos = startPos + steps;
        l_pos = startPos - (k - 2 * steps);
        start = bl(positions, fruitsSize, l_pos);
        end = br(positions, fruitsSize, r_pos);
        ans = max(ans, num_fruits[end] - num_fruits[start]);
    }
    free(positions);
    free(num_fruits);
    return ans;
}
