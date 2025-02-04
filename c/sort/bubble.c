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

int main()
{
    int a[SIZE], t;
    srand((unsigned) time(NULL));
    for (int i = 0; i < SIZE; a[i] = rand() % (N + 1), i++);
#ifdef print
    for (int i = 0; i < SIZE; printf("%d ", a[i]), i++);
    printf("\n");
#endif
    unsigned int start_time = clock();
    for (int i = 0; i < (SIZE - 1); i++)
        for (int j = 0; j < (SIZE - i - 1); j++)
            if (a[j] > a[j + 1])
            {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
    unsigned int stop_time = clock();
    
#ifdef print
    for (int i = 0; i < SIZE; printf("%d ", a[i]), i++);
    printf("\n");
#endif    
    printf("%lf\n", (double)(stop_time - start_time) / 1000000.0);
    return 0;
}
