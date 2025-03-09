int numberOfAlternatingGroups(int* colors, int colorsSize, int k) {
    int start = -1, ans = 0, curr_window = 1;
    for (int i = 1; i < colorsSize; i++) {
        if (colors[i] == colors[i - 1]) {
            start = i;
            break;
        }
    }
    if (start == -1) {
        if (colors[0] != colors[colorsSize - 1]) return colorsSize;
        start = 0;
    }
    printf("%d\n", start);
    for (int i = start + 1; i < start + colorsSize; i++) {
        if (colors[i % colorsSize] == colors[(i - 1) % colorsSize]) {
            if (curr_window >= k) ans += curr_window - k + 1;
            curr_window = 1;
        } else {
            curr_window += 1;
        }
    }
    if (curr_window >= k) ans += curr_window - k + 1;
    return ans;
}
