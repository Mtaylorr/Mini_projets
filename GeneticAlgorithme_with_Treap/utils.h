
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

typedef struct _key{
    int value;
    int nb;
} Key;

typedef struct _individu{
    int prior;
    int subtree;
    Key key;
    int * DNA;
    int value;
    int nb;
    struct _individu * l;
    struct _individu * r;
}Individu;



Individu * getNewIndividu(int l, int *t);

int cmp_keys(Key a, Key b);

Individu * getCopy(Individu * indiv, int l);

int exist(Individu * gen , Individu * indiv, int l);

int subtree(Individu * indiv);

void upd_subtree(Individu *  indiv);

void split_key (Individu *t, Key key, Individu ** l, Individu **  r);

void split_position(Individu *t , Individu ** l, Individu **r, int key , int add);

void inser (Individu ** t,Individu * it);

void merge(Individu **t , Individu * l, Individu * r);

void fusion(Individu ** gen , Individu * childgen, int l);

void dest(Individu * ind);



Individu * getElementByIndex (Individu **gen, int idx);

Individu * GetBestElement(Individu *t);

void GetBestElements(Individu ** t, int key);

Individu * getInitialGeneration( int n, int* t, int l);

void  addChildGen(Individu ** gen , int n , int l, int *t);

void mutation(Individu * indiv, int l);

void croisement(Individu * father, Individu * mother , int *t, int l );

void printIndiv(Individu * indiv, int *t, int l);

int fitness(Individu * ind, int l, int t[]);

int fit_nb(Individu* indiv, int l);

int eq(Individu *  indiv1 , Individu * indiv2,int l);

int cmp(Individu *  indiv1 , Individu * indiv2);

int checkIndiv(Individu * indiv, int l);


#endif // UTILS_H_INCLUDED
