#define min(a, b)  (a) < (b) ? (a) : (b);

int minimumTotal(int** triangle, int triangleSize, int* triangleColSize) {
    for (int rn = 1; rn < triangleSize; rn++) {
        triangle[rn][0] += triangle[rn - 1][0];
        triangle[rn][triangleColSize[rn] - 1] += triangle[rn - 1][triangleColSize[rn - 1] - 1];
        for (int en = 1; en < triangleColSize[rn] - 1; en++)
            triangle[rn][en] += min(triangle[rn - 1][en - 1], triangle[rn - 1][en]);
    }
    int ans = triangle[triangleSize - 1][0];
    for (int en = 1; en < triangleColSize[triangleSize - 1]; en++)
        ans = min(ans, triangle[triangleSize - 1][en]);
    return ans;
}
