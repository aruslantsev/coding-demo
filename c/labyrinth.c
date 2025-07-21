#include <stdio.h>
#include <stdlib.h>


void lab(int, int, int *, int, int, bool *);


int main()
{
    FILE *infile;
    int MM, NN, X, Y, *l;
    bool *ex;
    
    ex = (bool *) malloc(sizeof(bool));
    *ex = false;    
    
    infile = fopen("labyrinth.in", "r"); 
    /* Структура файла: первая строка - числа MM и NN - величина поля лабиринта, вторая строка - числа X и Y - стартовая позиция. Далее идут NN строк длиной MM с числами 0 (ячейка доступна для прохода) и 1 (проход перекрыт) */
    fscanf(infile, "%d%d", &MM, &NN);
    fscanf(infile, "%d%d", &X, &Y);
    X--;
    Y--;
    l = (int *) malloc(MM * NN * sizeof(int));
    for (int i = 0; i < NN; i++)
        for(int j = 0; j < MM; j++)
        {
            int a;
            fscanf(infile, "%d", &a);
            *(l + i * MM + j) = a;
        }
    
    fclose(infile);
    
    for (int i = 0; i < NN; i++)
    {
        for(int j = 0; j < MM; printf("%2d ", *(l + i * MM + j)), j++);
        printf("\n");
    }
    
    
    if (*(l + X * MM + Y) == 1)
        printf("Нет выхода\n");
    else
    {
        lab(X, Y, l, MM, NN, ex);
        if (!*ex)
            printf("Нет выхода\n");
    }
    return 0;
}

void lab(const int x, const int y, int *l, const int mm, const int nn, bool *ex)
{
    if (!*ex)
    {
    if ((*(l + y * mm + x) == 0) && (x >= 0) && (x <= mm - 1) && (y >= 0) && (y <= nn - 1))
    {
        if ((x == 0) || (x == mm - 1) || (y == 0) || (y == nn - 1))
            *ex = true;
        else if (!*ex)
        {
            *(l + y * mm + x) = 1;
            lab(x + 1, y, l, mm, nn, ex);
            lab(x - 1, y, l, mm, nn, ex);
            lab(x, y + 1, l, mm, nn, ex);
            lab(x, y - 1, l, mm, nn, ex);           
        }
    }
    if (*ex)
        printf("%d %d\n", x + 1, y + 1);
    }
    return;
}
