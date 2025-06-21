int minimumDeletions(char* word, int k) {
    int freq[26] = {0};
    for (int i = 0; word[i] != '\0'; freq[word[i] - 'a']++, i++);
    int ans = strlen(word);
    int minimum, curr, deleted;
    for(int i = 0; i < 26; i++) {
        if (freq[i] == 0) continue;
        minimum = freq[i];
        deleted = 0;
        for (int j = 0; j < 26; j++) {
            curr = freq[j];
            if (curr == 0) continue;
            if (curr < minimum) {
                deleted += curr;
            } else if (curr - minimum > k) {
                deleted += curr - minimum - k;
            }
        }
        if (deleted < ans) ans = deleted;
    }
    return ans;
}
