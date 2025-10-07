int compareVersion(char* version1, char* version2) {
    char *tok1 = strsep(&version1, ".");
    char *tok2 = strsep(&version2, ".");
    int val1, val2;
    while (tok1 || tok2) {
        if (tok1) val1 = atoi(tok1);
        else val1 = 0;
        if (tok2) val2 = atoi(tok2);
        else val2 = 0;
        if (val1 > val2) return 1;
        else if (val1 < val2) return -1;
        if (tok1) tok1 = strsep(&version1, ".");
        if (tok2) tok2 = strsep(&version2, ".");
    }
    return 0;
}
