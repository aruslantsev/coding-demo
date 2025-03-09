int minimumRecolors(char* blocks, int k) {
    int curr = 0, ans;
    for (int i = 0; (i < k) && (blocks[i] != '\0'); i++)
        if (blocks[i] == 'W') curr++;
    ans = curr;
    for (int i = k; blocks[i] != '\0'; i++) {
        if (blocks[i] == 'W') curr++;
        if (blocks[i - k] == 'W') curr--;
        ans = curr < ans? curr: ans;
    }
    return ans;
}
