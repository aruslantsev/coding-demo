int maxDifference(char* s) {
    int cnt[26] = {0}, max_odd = 0, min_even = 101;
    for (int i = 0; s[i] != '\0'; i++) cnt[s[i] - 'a']++;
    for (int i = 0; i < 26; i++) {
        if (cnt[i] > 0) {
            if (cnt[i] % 2 == 0 & cnt[i] < min_even) min_even = cnt[i];
            if (cnt[i] % 2 == 1 & cnt[i] > max_odd) max_odd = cnt[i];
        }
    }
    return max_odd - min_even;
}
