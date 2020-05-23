#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <stdbool.h>

Individu * getNewIndividu(int l, int *t){
    // generates a new individual by random
    Individu * indv  = malloc(sizeof(Individu)); // initialize variable
    indv->value = 0;
    indv->next= NULL;
    indv->nb = 0;
    indv->DNA = malloc(l*sizeof(int));
    int cnt=0;
    for(int i=0;i<l;i++){
        int ra = rand()%60; // choice of DNA with 1/3 probability for every type
        if(ra<20){//20
            indv->DNA[i]=0;
            indv->nb++;
        }else if(ra<40){//40
            indv->DNA[i]=1;
            cnt++;
        }else{
            indv->nb++;
            indv->DNA[i]=2;
        }
        indv->value+=(indv->DNA[i]-1)*t[i];// sum of all elements corresponding to the current configuration
    }
    indv->value = abs(indv->value); //  take the absolute value of the sum
    if(cnt==l){
        // if the current individual has no elements in his set , then we free the memory and we try again
        free(indv);
        return getNewIndividu(l, t);
    }
    return indv;
}

Individu * getCopy(Individu * indiv, int l){
    // to copy the individual without it's next element in the list
    Individu * indv  = malloc(sizeof(Individu));
    indv->value = indiv->value;
    indv->next= NULL;
    indv->nb=indiv->nb;
    indv->DNA = malloc(l*sizeof(int));
    for(int i=0;i<l;i++){
        indv->DNA[i] = indiv->DNA[i];
    }
    return indv;
}

Individu * getElementByIndex (Individu * gen , int idx){
    // to return the pointer corresponding the element in the idx'th position
    //printf("idx = %d\n",idx);
    if(idx == 0)return gen; //  if idx = 0 then we return the current element
    return getElementByIndex(gen->next, --idx); //  otherwise we decrease the index by 1 and apply the same function to the next element
}

Individu * GetBestElements(Individu * gen, int k){
    // to return best k elements in the linked list , but our linked list is always sorted thanks the the inser function , we just need to return the first k element
    if(gen==NULL)return NULL;
    if(k>0){
        gen->next = GetBestElements(gen->next, k-1);
        return gen;
    }else{
        gen->next =  GetBestElements(gen->next, k-1); // we apply the function here just to free the memory of the rest of the linked list
        free(gen);
        return NULL;
    }
}

Individu * inser(Individu * gen , Individu * indiv, int l){
    // insert the indv element in the gen in his correct position ==> sorted linked list
    if(gen==NULL){
        return indiv;
    }
    if(cmp(indiv, gen)){ //  cmp : it's a function to compare two individuals cmp(a,b) is true iff a is better than b
        indiv->next = gen;
        return indiv;
    }
    Individu * pcrawl = gen; // pointer to go through the linked list
    Individu * last = gen; //  last visited element in the linked list
    while(1){
        if(pcrawl == NULL){
            // if we reached the end of the linked list we just append it to the last element
            last->next = indiv;
            return gen;
        }else{
            if(!cmp(indiv, pcrawl)){
                if(eq(indiv, pcrawl,l))return gen;
                // if the element in pcrawl is better then our new indiv we just keep going
                last = pcrawl;
                pcrawl = pcrawl->next;
            }else{
                // otherwise we append it before pcrawl that mean to the element before pcrawl which is "last"
                last->next = indiv;
                indiv->next = pcrawl;
                return gen;
            }
        }
    }
    return gen;
}

Individu * getInitialGeneration( int n, int* t, int l){
    // to generate the initial generation with n individuals
    Individu * gen = getNewIndividu(l,t); //  get the first individual
    for(int i=0;i<n-1;i++){
        // append more n-1 individuals
        gen = inser(gen,getNewIndividu(l,t),l);
    }
    return gen;
}



Individu * addChildGen(Individu * gen , int n , int l, int *t){
    // in this function we will generate the children generation from the best (n/2) individuals randomly
    // then we will merge the child generation and parent generation
    // ==> hybrid generation
    Individu  * childGen = NULL;
    double a[n+1];
    double s = 1.7;
    int lambda = n;
    for(int i=0;i<n;i++){
        a[i]=(2.0-s)/(double)(n*1.0) + (2.0*(n-i-1)*(s-1))/(double)(1.0*n*(n-1));
        if(i>0)a[i]+=a[i-1];
    }
    double r = ((double)rand() / (double)RAND_MAX ) /((double)1.0*lambda);
    //printf(" r = %.10lf\n",r);
    int j=0;
    int current_member=0;
    while(current_member<lambda){
        while(r<=a[j] && current_member<lambda){
            int father_idx =current_member ;// choose the index of the father randomly
            int mother_idx = j;
            /*do{
                // choose the index of the mother randomly different from the father's one
                mother_idx = rand()%(n/2);
            }while(mother_idx==father_idx);*/
            //printf("%d\n",j);
            //printf("  %.10lf\n %d ",r,j);
            Individu * father = getCopy(getElementByIndex(gen,father_idx),l); //  get a copy from the father
            Individu * mother = getCopy(getElementByIndex(gen,mother_idx),l); //  get a copy from the mother

            croisement(father, mother, t , l); // apply crossover

            if(checkIndiv(father,l))childGen = inser(childGen, father,l); // if it's valid append it the child generation
            if(checkIndiv(mother,l))childGen = inser(childGen, mother,l); // if it's valid append it the child generation

            r = r + (1.0/(double)lambda);
            current_member++;
        }
        j++;
    }
    for(int i=0;i<n;i++){
        int ra =  rand()%1000;
       // printf("ra = %d\n",ra);
       // probability 8% to do mutation otherwise crossover
        if(ra<=80){
            int idx = rand()%(n/2); //  choose index randomly from the best (n/2) elements
            Individu * to_mutate = getCopy(getElementByIndex(gen,idx),l); // get a copy of the individual
            mutation(to_mutate, l); // apply mutation to the individual
            to_mutate->value = fitness(to_mutate, l , t); //  update fitness value
            to_mutate->nb = fit_nb(to_mutate, l); //  update number of elements taken from the set
            if(checkIndiv(to_mutate,l))childGen = inser(childGen, to_mutate,l); //  if this individual is valid then we take it
                                                                    // and append it to the child generation
        }else{
            childGen = inser(childGen, getNewIndividu(l,t),l);
        }
    }

    while(childGen){
        Individu * indiv = getCopy(childGen,l); //  get a copy of the first element
        Individu * lastindiv = childGen; //  store the first element
        childGen = childGen->next; // we move by one position
        free(lastindiv); // destroy the first element
        gen = inser(gen , indiv,l); // add the child to the generation
    }
    return gen;
}


void mutation(Individu * indiv, int l){
    int pos  =  rand() % l; //  choose a position by random to apply mutation in it
    int ra = rand()%50; // there are two other possibilities so we are selection them equally
    if(ra<25)indiv->DNA[pos] = (indiv->DNA[pos] + 1)%3; // +1 and +2 modular 3 will give the other two possibilities
    else indiv->DNA[pos] = (indiv->DNA[pos] + 2)%3;
}


void croisement(Individu * father, Individu * mother , int *t, int l ){
    int ra = rand() % (l-1); // choose the position of the cut randomly
    father->value = 0;
    mother->value = 0;
    father->nb = 0;
    mother->nb = 0;
    father->next = NULL;
    mother->next = NULL;
    for(int i=0;i<l;i++){
        if(i<=ra){
            // if i <= cut position we swap the configuration of the father and the mother otherwise let it the same
            int aux = father->DNA[i];
            father->DNA[i]=mother->DNA[i];
            mother->DNA[i]=aux;
        }
        // updating parameters
        father->value += (father->DNA[i]-1)*t[i];
        father->nb += abs((father->DNA[i]-1));
        mother->value += (mother->DNA[i]-1)*t[i];
        mother->nb += abs((mother->DNA[i]-1));
    }
    father->value = abs(father->value);
    mother->value = abs(mother->value);
}

int fitness(Individu * ind , int l , int*t){
    // function to calculate the fitness of an individual
    int sum=0;
    for(int i=0;i<l;i++){
        sum+=(ind->DNA[i]-1)*t[i];
    }
    return abs(sum);
}

int fit_nb(Individu* indiv, int l){
    // function to calculate the number of taken numbers in a configuration of an individual
    int sum=0;
    for(int i=0;i<l;i++){
        if(indiv->DNA[i]==1)continue;
        sum++;
    }
    return sum;
}

int cmp(Individu *  indiv1 , Individu * indiv2){
    // function to compare 2 individuals
    // return true if indiv1 is better than indiv2
    if(indiv1->value != indiv2->value){
        // if they have different fitness values , the one with the lowest sum is better
        return indiv1->value < indiv2->value;
    }
    // otherwise the one with the maximum size set is better
    return indiv1->nb > indiv2->nb;
}
int eq(Individu *  indiv1 , Individu * indiv2, int l){
    for(int i=0;i<l;i++){
        if(indiv1->DNA[i]!=indiv2->DNA[i])return 0;
    }
    return 1;
}

int checkIndiv(Individu * indiv, int l){
    // to check if an individual have an empty set or not
    int cnt=0;
    for(int i=0;i<l;i++){
        if(indiv->DNA[i]==1)cnt++;
    }
    // return true if it's valid (not empty) otherwise false
    return (cnt!=l);
}


void printIndiv(Individu * indiv, int *t, int l){
    // function to show an individual
   printf("The score of this individual is %d  : ", indiv->value);
   int cnt=0;
    for(int i=0;i<l;i++){
        if(indiv->DNA[i]==1){
            cnt++;
            continue;
        }
        if(indiv->DNA[i]==0){
            printf(" - %d ", t[i]);
        }else{
            printf(" + %d ", t[i]);
        }
    }
    printf("\n");
    printf("set size = %d",l-cnt);
}




