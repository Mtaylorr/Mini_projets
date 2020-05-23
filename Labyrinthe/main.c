#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <time.h>
int main()
{
    srand(time(NULL));
    int n=20;
    int m=20;
    clock_t tic = clock();
    grid * g =  getnewgrid(n,m);
    generate_grid(g,0,0);
    /*for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            printf("%d ",g->t[i][j]);
        }
        printf("\n");
    }*/
    clock_t toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    show(g);


    return 0;
}
