#define max(a, b) (a) > (b)?(a) : (b)
#define min(a, b) (a) < (b)?(a) : (b)

int maxArea(int* height, int heightSize) {
    int maxarea = 0, h;
    int l = 0, r = heightSize - 1;
    while (l < r) {
        h = min(height[l], height[r]);
        maxarea = max(maxarea, h * (r - l));
        if (height[l] < height[r]) l++;
        else r--;
    }
    return maxarea;
}
