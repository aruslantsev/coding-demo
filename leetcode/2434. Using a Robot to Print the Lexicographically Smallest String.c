char* robotWithString(char* s) {
    int cnt[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    char curr_min = 'a';
    char *ans = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    char *stack = (char *) malloc(sizeof(char) * strlen(s));
    int sp = 0, ap = 0;
    for (int i = 0; s[i] != '\0'; cnt[s[i] - 'a']++, i++);
    for (int i = 0; s[i] != '\0'; i++) {
        stack[sp++] = s[i];
        cnt[s[i] - 'a']--;
        while (curr_min < 'z' && cnt[curr_min - 'a'] == 0) curr_min++;
        while (sp > 0 && stack[sp - 1] <= curr_min) ans[ap++] = stack[--sp];
    }
    ans[ap] = '\0';
    free(stack);
    return ans;
}
