#include <stdio.h>
#include <stdlib.h>

void positive(double *k)
{
    if (*k != 0)
        *k = *k > 0 ? *k : -(*k);
    return;
}

int main()
{
    double *n;
    n = (double *) malloc(sizeof(float));
    scanf("%lf", n);
    printf("Entered: %.2lf\n", *n);
    positive(n);
    printf("After applying function: %.2lf\n", *n);
    free(n);
    return 0;
}
