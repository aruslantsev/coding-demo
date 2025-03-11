int numberOfSubstrings(char* s) {
    int n = strlen(s);
    int ca, cb, cc, ans = 0;
    ca = cb = cc = n;
    for (int i = n - 1; i >= 0; i--) {
        if (s[i] == 'a') ca = i;
        if (s[i] == 'b') cb = i;
        if (s[i] == 'c') cc = i;
        ans += n - (ca > cb? (ca > cc? ca: cc): (cb > cc? cb: cc));
    }
    return ans;
}
