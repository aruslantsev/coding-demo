char* clearDigits(char* s) {
    int i, j = 0;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            j--;
        } else {
            s[j++] = s[i];
        }
    }
    s[j] = '\0';
    return s;
}
