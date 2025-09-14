#define max(a, b) (a) > (b)? a: b

bool is_vowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}

int maxFreqSum(char* s) {
    int cnt[26];
    int v = 0, c = 0;
    for(; *s != '\0'; s++)
        cnt[*s - 'a']++;
    for (int i = 0; i < 26; i++) {
        if (is_vowel('a' + i)) v = max(v, cnt[i]);
        else c = max(c, cnt[i]);
    }
    return v + c;
}
