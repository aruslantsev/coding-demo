char* clearStars(char* s) {
    int syms[26][100000];
    int sp[26] = {0};
    int ast = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] != '*') {
            syms[s[i] - 'a'][sp[s[i] - 'a']++] = i;
        }
        else {
            ast++;
            for (int j = 0; j < 26; j++) {
                if (sp[j]) {
                    s[syms[j][--sp[j]]] = '*';
                    break;
                }
            }
        }
    }
    char *ans = (char *) malloc(sizeof(char) * (strlen(s) + 1 - ast));
    int ap =  0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] != '*') ans[ap++] = s[i];
    }
    ans[ap] = '\0';
    return ans;
}
