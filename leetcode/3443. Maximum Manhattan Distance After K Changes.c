int maxDistance(char* s, int k) {
    int latitude = 0, longitude = 0, ans = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        switch (s[i]) {
            case 'N':
                latitude++;
                break;
            case 'S':
                latitude--;
                break;
            case 'E':
                longitude++;
                break;
            case 'W':
                longitude--;
                break;
        }
        ans = fmax(ans, fmin(i + 1, abs(latitude) + abs(longitude) + 2 * k));
    }
    return ans;
}
