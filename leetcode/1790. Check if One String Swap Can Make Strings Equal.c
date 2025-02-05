bool areAlmostEqual(char* s1, char* s2) {
    int num_diffs = 0, pos[2], i = 0;
    while (s1[i] != '\0') {
        if (s1[i] != s2[i]) {
            if (num_diffs == 2) return false;
            pos[num_diffs] = i;
            num_diffs++;
        }
        i++;
    }
    if (num_diffs == 0) return true;
    if (num_diffs == 1) return false;
    return ((s1[pos[1]] == s2[pos[0]]) & (s1[pos[0]] == s2[pos[1]]));
}
