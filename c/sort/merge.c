#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 30000
// #define print
#ifdef print
#define SIZE 5
#endif
#ifndef print
#define SIZE 30000
#endif


void mergesort(int* a, int size)
{
    if (size == 1)
        return;
    else
    {
        int *a1, *a2, size1, size2, m, n;
        size1 = (int) size / 2;
        size2 = size - size1;
        a1 = (int *) malloc(size1 * sizeof(int));
        a2 = (int *) malloc(size2 * sizeof(int));
        for (int i = 0; i < size1; a1[i] = a[i], i++);
        for (int i = size1; i < size1 + size2; a2[i - size1] = a[i], i++);
        mergesort(a1, size1);
        mergesort(a2, size2);
        m = 0;
        n = 0;
        for (int i = 0; i < size; i++)
        {
            if ((m < size1) && (n < size2))
            {
                if (a1[m] < a2[n])
                    a[i] = a1[m++];
                else
                    a[i] = a2[n++];
            }
            else
            {
                if ((n == size2) && (m < size1))
                    a[i] = a1[m++];
                if ((m == size1) && (n < size2))
                    a[i] = a2[n++];
            }
        }
        free(a1);
        free(a2);
        return;   
    }
    return;
}


int main()
{
    int *a;
    a = (int *) malloc(SIZE * sizeof(int));
    srand((unsigned)time(NULL));
    for (int i = 0; i < SIZE; a[i] = rand() % (N + 1), i++);
#ifdef print
    for (int i = 0; i < SIZE; printf("%d ", a[i]), i++);
    printf("\n");
#endif
    unsigned int start_time =  clock();    
    mergesort(a, SIZE);    
    unsigned int stop_time =  clock();
#ifdef print
    for (int i = 0; i < SIZE; printf("%d ", a[i]), i++);
    printf("\n");
#endif    
    printf("%lf\n", (double)(stop_time - start_time) / 1000000.0);
    free(a);
    return 0;
}
