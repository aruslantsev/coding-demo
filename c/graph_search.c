#include <stdio.h>
#include <stdlib.h>

void gsearch(int *gmap, int gsize, int *gstat, int nstart, int nstop, int *exit)
{
    if (!*exit)
    {
        if (nstart == nstop)
            *exit = 1;
        else
            if (*(gstat + nstart) == 0)
            {
                *(gstat + nstart) = 1;
                for (int i = 0; i < gsize; i++)
                    if ((*(gmap + nstart * gsize + i) == 1) && (nstart != i))
                        gsearch(gmap, gsize, gstat, i, nstop, exit);
            }
        if (*exit)
            printf("%d ", nstart);
    }
    
    return;
}

int main()
{
    int *map, *stat, size, nstart, nstop;
    FILE *infile;
    int *exit;
    infile = fopen("graph.in", "r"); 
    fscanf(infile, "%d", &size);
    fscanf(infile, "%d %d", &nstart, &nstop);
    map = (int *) malloc(size * size * sizeof(int));
    stat = (int *) malloc(size * sizeof(int));
    for(int i = 0; i < size; i++)
    {
        int s;
        for(int j = 0; j < size; j++)
        {
            fscanf(infile, "%d", &s);
            *(map + i * size + j) = s;
        }
        *(stat + i) = 0;
    }
    *exit = 0;
    
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
            printf("%d ", *(map + i * size + j));
        printf("\n");
    }
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            if (*(map + i * size + j) == 1)
                printf("%d -> %d\n", i, j);
    fclose(infile);
    
    gsearch(map, size, stat, nstart, nstop, exit);
    if (!*exit)
        printf("No path");
    printf("\n");
    return 0;
}
