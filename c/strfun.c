#include <stdio.h>

#define MAX_LEN 100

int is_good(char sym, char blacklist[]) {
    for (int j = 0; blacklist[j] != '\0'; j++)
        if (sym == blacklist[j])
            return 0;
    return 1;
}

void squeeze(char s1[], char s2[]) {
    /* Removes from s1 chars presented in s2 */
    int i = 0, j = 0;
    for (; s1[i] != '\0'; i++)
        if (is_good(s1[i], s2))
            s1[j++] = s1[i];
    s1[j] = '\0';
}

int any(char s1[], char s2[]) {
    /* Returns leftmost position of symbol in s1, which is in s2 too */
    for (int i = 0; s1[i] != '\0'; i++)
        for (int j = 0; s2[j] != '\0'; j++)
            if (s1[i] == s2[j])
                return i;
    return (int) -1;
}


int get_length(char s[], int max_len) {
    /* Computes length of s, converts multiple spaces to one */
    int sym, prev = ' ', i = 0;
    while ((i < max_len) && ((sym = getchar()) != '\n') && (sym != EOF)) {
        if ((sym != ' ') || (prev != ' ')) {
            s[i] = (char)sym;
            i++;
        }
        prev = sym;
    }
    s[i] = '\0';
    return i;
}

void copy(char from[], char to[]) {
    /* copies from -> to */
    for (int i = 0; (to[i] = from[i]) != '\0'; i++);
}
