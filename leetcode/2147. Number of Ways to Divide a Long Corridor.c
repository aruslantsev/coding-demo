int numberOfWays(char* corridor) {
    char *s;
    size_t counter;
    int num_seats = 0, window = 0;
    for(counter = 0, s = corridor; *s != '\0'; s++)
        if (*s == 'S') num_seats++;
    if (num_seats % 2 == 1 || num_seats == 0) return 0;
    char *start, *end;
    start = corridor;
    while (*start == 'P') start++;
    end = start;
    while (num_seats > 0) {
        if (*end == 'S') num_seats--;
        end++;
    }

    long ans = 1;
    for (s = start; s < end; s++) {
        if (*s == 'S') {
            if (window > 0) {
                ans = (ans * (window + 1) % 1000000007);
                window = 0;
            }
            num_seats = (num_seats + 1) % 2;
        } else {
            if (num_seats == 0) window++;
        }
    }

    return (int) ans;
}
