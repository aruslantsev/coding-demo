#include <stdio.h>
#include <stdlib.h>

void positive(float *k)
{
    if (*k != 0)
        *k = *k > 0 ? *k : -(*k);
    return;
}

int main()
{
    float *n;
    n = (float *) malloc(sizeof(float));
    scanf("%f", n);
    positive(n);
    printf("%f\n", *n);
    free(n);
    return 0;
}
