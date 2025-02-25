int numOfSubarrays(int* arr, int arrSize) {
    int num_even = 1, num_odd = 0, curr_sum = 0, ans = 0;
    for (int i = 0; i < arrSize; i++) {
        curr_sum += arr[i];
        if (curr_sum % 2 == 1) {
            ans += num_even;
            num_odd++;
        } else {
            ans += num_odd;
            num_even++;
        }
        ans %= 1000000007;
    }
    return ans;
}
