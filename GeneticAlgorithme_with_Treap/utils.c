#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

Individu * getNewIndividu(int l, int *t){
    Individu * indv  = malloc(sizeof(Individu));
    indv->subtree=1;
    indv->prior = rand();
    indv->value = 0;
    indv->l=indv->r= NULL;
    indv->nb = 0;
    indv->DNA = malloc(l*sizeof(int));
    int cnt=0;
    for(int i=0;i<l;i++){
        int ra = rand()%60;
        if(ra<20){
            indv->DNA[i]=0;
            indv->nb++;
        }else if(ra<40){
            indv->DNA[i]=1;
            cnt++;
        }else{
            indv->nb++;
            indv->DNA[i]=2;
        }
        indv->value+=(indv->DNA[i]-1)*t[i];
    }
    indv->value = abs(indv->value);
    if(cnt==l){
        free(indv);
        return getNewIndividu(l, t);
    }
    indv->key.value = indv->value;
    indv->key.nb = indv->nb;
    return indv;
}

int cmp_keys(Key a, Key b){
    if(a.value!=b.value){
        return a.value<b.value;
    }
    return a.nb>b.nb;
}


Individu *  getCopy(Individu * indiv, int l){
    Individu * indv  = malloc(sizeof(Individu));
    indv->prior = rand();
    indv->subtree = 1;
    indv->value = indiv->value;
    indv->l=indv->r= NULL;
    indv->nb=indiv->nb;
    indv->DNA = malloc(l*sizeof(int));
    for(int i=0;i<l;i++){
        indv->DNA[i] = indiv->DNA[i];
    }
    indv->key.value = indv->value;
    indv->key.nb = indv->nb;
    return indv;
}

int exist(Individu * gen , Individu * indiv, int l){
    if(gen==NULL)return 0;
    if(eq(gen, indiv,l))return 1;
    return exist(cmp_keys(indiv->key , gen->key)?gen->l:gen->r,indiv, l);
}


int subtree(Individu * indiv){
    return indiv? indiv->subtree :0;
}

void upd_subtree(Individu *  indiv){
    if(indiv){
        indiv->subtree = subtree(indiv->l) + subtree(indiv->r) + 1;
    }
}



void split_key (Individu * t, Key key, Individu ** l, Individu **  r) {
    if (!t)
        *l = *r = NULL;
    else if (cmp_keys(key , t->key))
        split_key (t->l, key, l, &(t->l)),  *r = t;
    else
        split_key (t->r, key, &(t->r), r),  *l = t;
    upd_subtree(t);
}

void split_position(Individu *t , Individu ** l , Individu **r, int key , int add){
    if(!t){
        *l=NULL;
        *r = NULL;
        return ;
    }else{
        int cur_key = add + subtree(t->l);
        //printf("cur_key %d %d\n",cur_key,key );
        if(key<=cur_key)
            split_position(t->l,l,&(t->l),key,add),*r=t;
        else
            split_position(t->r, &(t->r),r,key, add+1 + subtree(t->l)),*l=t;
    }
    upd_subtree(t);
}


void inser (Individu ** t,Individu * it) {
    if (!(*t))
        *t = it;
    else if (it->prior > (*t)->prior)
        split_key(*t, it->key, &(it->l), &(it->r)),  *t = it;
    else
        inser(cmp_keys(it->key, (*t)->key) ? &((*t)->l) : &((*t)->r), it);
    upd_subtree(*t);
}


void merge(Individu **t , Individu * l, Individu * r){
    if(!l || !r){
        *t= l?l:r;
    }else if(l->prior > r->prior)
        merge(&(l->r),l->r,r), *t=l;
    else
        merge(&(r->l),l,r->l),*t=r;
    upd_subtree(*t);
}

void fusion(Individu ** gen , Individu * childgen, int l){
    if(childgen==NULL)return ;
    Individu * to_insert = getCopy(childgen,l);
    if(exist(*gen,to_insert,l)==0)
        inser(gen,to_insert);
    else free(to_insert);
    fusion(gen,childgen->l,l);
    fusion(gen,childgen->r,l);
}

void dest(Individu * indiv){
    if(indiv==NULL)return ;
    dest(indiv->l);
    dest(indiv->r);
    free(indiv);
}


Individu * getElementByIndex (Individu **gen, int idx){
    Individu * t1 = malloc(sizeof(Individu));
    Individu * t2 = malloc(sizeof(Individu));
    Individu * t3 = malloc(sizeof(Individu));
    split_position(*gen,  &t1, &t2,idx,0);
    split_position(t2, &t2, &t3,1,0);
    Individu * to = t2;
    merge (gen, t1, t2);
    merge (gen, *gen, t3);
    return to;
}


Individu * GetBestElement(Individu * ind){
    if(ind->l==NULL)return ind;
    return GetBestElement(ind->l);
}


void GetBestElements(Individu ** ind, int key){
    if(!(*ind))return ;
    Individu * p1 = malloc(sizeof(Individu));
    Individu * p2 = malloc(sizeof(Individu));
    split_position(*ind, &p1,&p2, key,0);
    *ind = p1;
}


Individu * getInitialGeneration( int n, int* t, int l){

    Individu * gen = getNewIndividu(l,t);
    for(int i=0;i<n-1;i++){
        inser(&gen,getNewIndividu(l,t));
    }
    return gen;
}

void addChildGen(Individu ** gen , int n , int l, int *t){
    //printf("childgen\n");
    Individu  * childGen = NULL;
    double a[n+1];
    double s = 1.7;
    int lambda = n-1;
    for(int i=0;i<n;i++){
        a[i]=(2.-s)/(n*1.0) + (2.0*(n-i-1)*(s-1))/(1.0*n*(n-1));
        if(i>0)a[i]+=a[i-1];
    }
    double r = ((double)rand() / (double)RAND_MAX ) /((double)1.0*lambda);
    int j=0;
    int current_member=0;
    while(current_member<lambda){
        while(r<=a[j] && current_member<lambda){
            int father_idx =current_member ;// choose the index of the father randomly
            int mother_idx = j;
            Individu * father = getCopy(getElementByIndex(gen,father_idx),l);
            Individu * mother = getCopy(getElementByIndex(gen,mother_idx),l);
            croisement(father, mother, t , l);
            if(checkIndiv(father,l)&& exist(childGen,father,l)==0)inser(&childGen, father);
            else free(father);
            if(checkIndiv(mother,l)&& exist(childGen,mother,l)==0)inser(&childGen, mother);
            else free(mother);

            r = r + (1/(double)lambda);
            current_member++;
        }
        j++;
    }

    for(int i=0;i<n/2;i++){
        int ra =  rand()%1000;
        //printf("ra = %d\n",ra);
        if(ra<=80){
            int idx = rand()%(n/2);
            Individu * to_mutate = getCopy(getElementByIndex(gen,idx),l);
            mutation(to_mutate, l);
            to_mutate->value = fitness(to_mutate, l , t);
            to_mutate->nb = fit_nb(to_mutate, l);
            if(checkIndiv(to_mutate,l) && exist(childGen,to_mutate,l)==0)inser(&childGen, to_mutate);
            else free(to_mutate);
        }else{
            Individu * new_indiv = getNewIndividu(l,t);
            if(exist(childGen,new_indiv,l))inser(&childGen, new_indiv);
            else free(new_indiv);
        }
    }
    fusion(gen , childGen,l);
    dest(childGen);
    GetBestElements(gen,n);

}



void mutation(Individu * indiv, int l){
    int pos  =  rand() % l;
    int ra = rand()%50;
    if(ra<25)indiv->DNA[pos] = (indiv->DNA[pos] + 1)%3; // +1
    else indiv->DNA[pos] = (indiv->DNA[pos] + 2)%3; // +2
}

void croisement(Individu * father, Individu * mother , int *t, int l ){
    int ra = rand() % (l-1);
    father->value = 0;
    mother->value = 0;
    father->nb = 0;
    mother->nb = 0;
    father->l=father->r = NULL;
    mother->l=mother->r = NULL;
    for(int i=0;i<l;i++){
        if(i<=ra){
            int aux = father->DNA[i];
            father->DNA[i]=mother->DNA[i];
            mother->DNA[i]=aux;
        }
        father->value += (father->DNA[i]-1)*t[i];
        father->nb += abs((father->DNA[i]-1));
        mother->value += (mother->DNA[i]-1)*t[i];
        mother->nb += abs((mother->DNA[i]-1));
    }
    father->value = abs(father->value);
    mother->value = abs(mother->value);
}

void printIndiv(Individu * indiv, int *t, int l){
    printf("The score of this individu is %d  : ", indiv->value);
    int cnt=0;
    //printf("%d",sizeof(t)/sizeof(int));
    for(int i=0;i<l;i++){
           // printf("%d \n",t[i]);
        if(indiv->DNA[i]==1){
            cnt++;
            continue;
        }
        if(indiv->DNA[i]==0){
            printf(" + %d ", t[i]);
        }else{
            printf(" - %d ", t[i]);
        }
    }
    printf("\n");
    printf("set size = %d",l-cnt);
}


int fitness(Individu * ind , int l , int*t){
    int sum=0;
    for(int i=0;i<l;i++){
        sum+=(ind->DNA[i]-1)*t[i];
    }
    return abs(sum);
}

int fit_nb(Individu* indiv, int l){
    int sum=0;
    for(int i=0;i<l;i++){
        if(indiv->DNA[i]==1)continue;
        sum++;
    }
    return abs(sum);
}

int cmp(Individu *  indiv1 , Individu * indiv2){
    if(indiv1->value != indiv2->value){
        return indiv1->value < indiv2->value;
    }
    return indiv1->nb > indiv2->nb;
}

int eq(Individu *  indiv1 , Individu * indiv2, int l){
    for(int i=0;i<l;i++){
        if(indiv1->DNA[i]!=indiv2->DNA[i])return 0;
    }
    return 1;
}

int checkIndiv(Individu * indiv, int l){
    int cnt=0;
    for(int i=0;i<l;i++){
        if(indiv->DNA[i]==1)cnt++;
    }
    return (cnt!=l);
}




