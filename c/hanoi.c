#include <stdio.h>

void hanoi(int a, int b, int c, int n)
{
    if (n > 2)
    {
        hanoi(a, c, b, n - 1);
        printf("%d -> %d\n", a, b);
        hanoi(c, b, a, n - 1);
    }
    else
    {
        printf("%d -> %d\n", a, c);
        printf("%d -> %d\n", a, b);
        printf("%d -> %d\n", c, b);
    }
    return;
}

int main()
{
    int n;
    printf("Введите количество дисков: ");
    scanf("%d", &n);
    if (n > 1)
        hanoi(1, 2, 3, n);
    else printf("1 -> 2\n");
    return 0;
}
