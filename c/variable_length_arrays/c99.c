#include <stdio.h>
#include <time.h>

void delay(int number_of_seconds)
{
    int time_diff = CLOCKS_PER_SEC * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + time_diff);
}

void test(long k) {
    long  var = k, arr[k]; /* variable length array, should be c99 and newer */
    printf("%lld %lld\n", k, var);
    for (var = 0; var < k; printf("%ld ", arr[var]), var++);
    printf("\n");
    for (var = 0; var < k; arr[var] = var, var++);
    for (var = 0; var < k; printf("%ld ", arr[var]), var++);
    printf("\n");
}

int main(void) {
    test(1);
    test(2);
    test(20);
    test(1000);
    test(1000000);  /* Segmentation fault if more than 1e6 */
    printf("WAIT\n");
    delay(10);
    test(1);
    printf("WAIT\n");
    delay(10);
    printf("WAIT\n");
    test(1000000);
    delay(10);
    return 0;
}
