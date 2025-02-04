#include <stdio.h>

int main()
{
    unsigned int n, i = 0;
    int b[20];
    printf("Введите положительное число : ");
    scanf("%d", &n);
    if (n == 0)
        printf("0");
    while (n > 0)
    {
        b[i] = n % 2;
        n = (int)(n / 2);
        i++;
    }
    for (int j = i - 1; j >= 0; printf("%d", b[j]), j--);
    printf("\n");
    return 0;
}
