int areaOfMaxDiagonal(int** dimensions, int dimensionsSize, int* dimensionsColSize) {
    int x, y, area, diag, max_diag = 0;
    for (int i = 0; i < dimensionsSize; i++) {
        x = dimensions[i][0];
        y = dimensions[i][1];
        if ((diag = x * x + y * y) > max_diag) {
            max_diag = diag;
            area = x * y;
        } else if (diag == max_diag) {
            area = x * y > area? x * y: area;
        }
    }
    return area;
}
