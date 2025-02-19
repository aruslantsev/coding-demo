#include <stdio.h>

#define NUM_DIGITS 10
#define NUM_LETTERS 26
#define MAX_STR_LEN 80.0

int main(void) {
    int nother = 0, nspaces = 0, digits[NUM_DIGITS], letters[NUM_LETTERS], curr, i, j, max_val = 0;
    float coef = 1.0;
    for (i = 0; i < NUM_DIGITS; digits[i] = 0, i++);
    for (i = 0; i < NUM_LETTERS; letters[i] = 0, i++);

    while ((curr = getchar()) != EOF) {
        if (curr >= '0' && curr <= '9') {
            ++digits[curr - '0'];
        } else if (curr >= 'a' && curr <= 'z') {
            ++letters[curr - 'a'];
        } else if (curr >= 'A' && curr <= 'Z') {
            ++letters[curr - 'A'];
        } else if (curr == ' ' || curr == '\t') {
            nspaces++;
        } else {
            nother++;
        }
    }

    for (i = 0; i < NUM_DIGITS; max_val = digits[i] > max_val? digits[i]: max_val, i++);
    for (i = 0; i < NUM_LETTERS; max_val = letters[i] > max_val? letters[i]: max_val, i++);
    max_val = nspaces > max_val? nspaces: max_val;
    max_val = nother > max_val? nother: max_val;

    if (max_val > 0)
        coef = MAX_STR_LEN / max_val;

    for (i = 0; i < NUM_DIGITS; i++) {
        printf("%d  |", i);
        for (j = 0; j < digits[i] * coef; printf("#"), j++);
        printf("\n");
    }
    for (i = 0; i < NUM_LETTERS; i++) {
        printf("%c  |", (char) ('a' + i));
        for (j = 0; j < letters[i] * coef; printf("#"), j++);
        printf("\n");
    }
    printf("' '|");
    for (j = 0; j < nspaces * coef; printf("#"), j++);
    printf("\n");
    printf("OTH|");
    for (j = 0; j < nother * coef; printf("#"), j++);
    printf("\n");

    return 0;
}
