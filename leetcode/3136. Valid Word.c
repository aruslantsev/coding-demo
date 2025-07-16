bool isValid(char* word) {
    if (strlen(word) < 3) return false;
    bool has_consonant = false, has_vowel = false;
    for (; *word; word++) {
        if (!isalnum(*word)) return false;
        if (isalpha(*word)) {
            char c = tolower(*word);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                has_vowel = true;
            } else {
                has_consonant = true;
            }
        }
    }
    return has_vowel && has_consonant;
}
