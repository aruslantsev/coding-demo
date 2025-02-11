char* removeOccurrences(char* s, char* part) {
    int pl = strlen(part), pos, i;
    char* entry;
    if (pl == 0) return s;
    while ((entry = strstr(s, part)) != NULL) {
        pos = entry - s;
        for (i = pos + pl; s[i] != '\0'; s[i - pl] = s[i], i++);
        s[i - pl] = '\0';
    }
    return s;
}
