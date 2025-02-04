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
    int a[SIZE], t, indexmax, max;
    srand((unsigned)time(NULL));
    for (int i = 0; i < SIZE; a[i] = rand() % (N + 1), i++);
#ifdef print
    for (int i = 0; i < SIZE; printf("%d ", a[i]), i++);
    printf("\n");
#endif
    unsigned int start_time =  clock();
    for (int i = 0; i < (SIZE - 1); i++)
    {
        indexmax = 0;
        max = a[0];
        for (int j = 0; j < (SIZE - i); j++)
            if (a[j] > max)
            {
                max = a[j];
                indexmax = j;
            }
        t = a[SIZE - 1 - i];
        a[SIZE - 1 - i] = max;
        a[indexmax] = t;
    }
    unsigned int stop_time =  clock();
#ifdef print
    for (int i = 0; i < SIZE; printf("%d ", a[i]), i++);
    printf("\n");
#endif
    printf("%lf\n", (double)(stop_time - start_time) / 1000000.0);
    return 0;
}
