int backtrack(uint8_t counter[]) {
    int ans = 0;
    for (int i = 0; i < 26; i++) {
        if (counter[i] > 0) {
            counter[i]--;
            ans += 1 + backtrack(counter);
            counter[i]++;
        }
    }
    return ans;
}

int numTilePossibilities(char* tiles) {
    uint8_t counter[26];
    for (int i = 0; tiles[i] != '\0'; counter[tiles[i] - 'A']++, i++);
    return backtrack(counter);
}
