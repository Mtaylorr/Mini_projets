#include <stdbool.h>
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

typedef struct _individu{
    int * DNA; // configuration
    int value; // fitness value
    int nb; // number of "on" elements in the set
    struct _individu * next; // next element in the linked list
}Individu;

Individu * getNewIndividu(int l, int *t);

Individu * getCopy(Individu * indiv, int l);

Individu * getElementByIndex (Individu* gen , int idx);

Individu * GetBestElements(Individu * gen, int k);

Individu * inser(Individu * get, Individu * indiv, int l);

Individu * getInitialGeneration( int n, int* t, int l);

Individu * addChildGen(Individu * gen , int n , int l, int *t);

void mutation(Individu * indiv, int l);

void croisement(Individu * father, Individu * mother , int *t, int l );

int fitness(Individu * ind, int l, int t[]);

int fit_nb(Individu* indiv, int l);

int cmp(Individu *  indiv1 , Individu * indiv2);

int eq(Individu *  indiv1 , Individu * indiv2,int l);

int checkIndiv(Individu * indiv, int l);

void printIndiv(Individu * indiv, int *t, int l);


#endif // UTILS_H_INCLUDED
