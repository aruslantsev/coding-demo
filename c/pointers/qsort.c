#include "sort_lines.h"


static void swap(char **, long unsigned int, long unsigned int);
static short int cmp(char [], char []);


void qsort(char *arr[], long unsigned int left, long unsigned int right) {
    long unsigned last = left, cnt;
    if (left >= right)
        return;
    swap(arr, left, (left + right) / 2);
    for (cnt = left + 1; cnt < right; cnt++)
        if (cmp(arr[left], arr[cnt]) > 0)
            swap(arr, ++last, cnt);
    swap(arr, last, left);
    qsort(arr, left, last);
    qsort(arr, last + 1, right);
}


void bubblesort(char *arr[], long unsigned int left, long unsigned int right) {
    long unsigned int i, j;
    for (i = right - 1; i > 0; i--)
        for (j = 0; j < i; j++)
            if (cmp(arr[j], arr[j + 1]) > 0)
                swap(arr, j, j + 1);
}


static void swap(char *arr[], long unsigned int a, long unsigned int b) {
    char *t;
    t = arr[a];
    arr[a] = arr[b];
    arr[b] = t;
}


static short int cmp(char left[], char right[]) {
    long unsigned i;
    for (i = 0; left[i] != '\0' && right[i] != 0; i++) {
        if (left[i] < right[i])
            return -1;
        else if (left[i] > right[i])
            return 1;
    }
    if (left[i] == '\0' && right[i] != '\0')
        return -1;
    if (left[i] != '\0' && right[i] == '\0')
        return 1;
    return 0;
}
