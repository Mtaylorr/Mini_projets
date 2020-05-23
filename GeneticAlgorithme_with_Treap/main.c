#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <time.h>


int main()
{
    int *t ; //  the elements of the set
    int n;  // number of units in the generation
    int l; //  the size of the set
    Individu * gen;
    Individu * best;
    int bestScore=10000000000;
    int num_iteration;
    srand(time(NULL));
    FILE * f;
    f=fopen("test0.txt","r");
    printf("The size of the set : ");
    fscanf(f,"%d",&l);
    t = malloc(l*sizeof(int));

    printf("%d\n",l);
    for(int i=0;i<l;i++){
        printf("the %d 'th elemenf of the set  : ",i);
        fscanf(f,"%d",&t[i]);
        printf("%d\n",*(t+i));

    }

    fclose(f);

    printf("Number of units par generation  : ");
    scanf("%d",&n);
    printf("Number of iteration  : ");
    scanf("%d",&num_iteration);

    gen = getInitialGeneration(n,t,l);
    best = GetBestElement(gen);
    bestScore = best->value;
    clock_t tic = clock();
    for(int i=0;i<num_iteration;i++){
        addChildGen(&gen,n,l,t);
        Individu * indv = GetBestElement(gen);
        if(cmp(indv,best)){
            bestScore = indv->value;
            best= getCopy(indv,l);
        }
    }
    clock_t toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    printf("finish\n");
    printf("bestScore = %d\n",bestScore);
    printIndiv(best,t,l);
    return 0;
}

