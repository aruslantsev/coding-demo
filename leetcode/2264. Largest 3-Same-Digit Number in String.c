char* largestGoodInteger(char* num) {
    char largest = '\0', prev = '\0';
    int cnt = 0;
    char *ans = (char *) malloc(4 * sizeof(char));
    ans[3] = '\0';
    for (; *num != '\0'; num++) {
        if (*num == prev) cnt++;
        else {
            cnt = 1;
            prev = *num;
        }
        if (cnt == 3) largest = (largest >= *num)? largest: *num;
    }
    ans[0] = ans[1] = ans[2] = largest;
    return ans;
}
