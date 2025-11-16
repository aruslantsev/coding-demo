int countOperations(int num1, int num2) {
    int t;
    int ans = 0;
    if (num2 > num1) {
        t = num1;
        num1 = num2;
        num2 = t;
    }
    while (num1 != 0 && num2 != 0) {
        ans += num1 / num2;
        t = num1 % num2;
        num1 = num2;
        num2 = t;
    }
    return ans;
}
