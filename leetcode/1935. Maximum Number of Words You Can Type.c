int canBeTypedWords(char* text, char* brokenLetters) {
    char *word = strtok(text, " ");
    int ans = 0;
    bool good;
    while (word) {
        good = true;
        for (char *c = brokenLetters; *c; ++c) {
            if (strchr(word, *c)) {
                good = false;
                break;
            }
        }
        if (good) ++ans;
        word = strtok(NULL, " ");
    }
    return ans;
}

int canBeTypedWords(char* text, char* brokenLetters) {
    int ans = 0;
    bool good = true;
    bool broken[26] = {false};
    for (; *brokenLetters; ++brokenLetters)
        broken[*brokenLetters - 'a'] = true;
    for (; *text; ++text) {
        if (*text == ' ') {
            if (good) ++ans;
            good = true;
        } else {
            if (good)
                if (broken[*text - 'a']) good = false;
        }
    }
    if (good) ++ans;
    return ans;
}
