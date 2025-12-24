#include <stdio.h>
#include <stdlib.h>


void fill_arr(int rows, int cols, int arr[rows][cols]);
void print_arr(int rows, int cols, int arr[rows][cols]);
void print_row(int [], int);

int main(void) {
    int m, n;
    puts("Enter number of rows and cols");
    scanf("%d%d", &m, &n);

    puts("\nCreating zero-filled array");
    int arr[m][n] = {};  /* only empty initialzers are allowed */
    size_t rows, cols, elems;

    puts("Calculating size");
    elems = sizeof(arr) / sizeof(int);
    cols = sizeof(arr[0]) / sizeof(int);
    rows = elems / cols;
    printf("Requested: %d elems, %d rows, %d cols.\n", m * n, m, n);
    printf("Got: %ld elems, %ld rows, %ld cols.\n", elems, rows, cols);

    puts("\nArray");
    print_arr(m, n, arr);
    puts("\nFilling up with random numbers");
    fill_arr(m, n, arr);
    puts("\nDone");
    print_arr(m, n, arr);

    puts("\nPrinting row by row");
    for (int i = 0; i < m; i++)
        print_row(arr[i], n);
    puts("");
    return 0;
}

void fill_arr(int rows, int cols, int arr[rows][cols]) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            arr[i][j] = rand() % 1000;
}

void print_arr(int rows, int cols, int arr[rows][cols]) {
    puts("");
    printf("%6s", "");
    for (int j = 0; j < cols; j++)
        printf("%5d", j);
    printf("%s", "\n\n");
    for (int i = 0; i < rows; i++) {
        printf("%5d|", i);
        for (int j = 0; j < cols; j++)
            printf("%5d", arr[i][j]);
        puts("");
    }
}

void print_row(int arr[], int elems) {
    for (int i = 0; i < elems; i++)
        printf("%5d", arr[i]);
    puts("");
}
