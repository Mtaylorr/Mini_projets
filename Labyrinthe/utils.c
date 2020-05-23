#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

grid * getnewgrid(int n , int m){
    grid *  to = malloc(sizeof(grid));
    to->n = n;
    to->m = m;
    to->t = (int** ) malloc(n*sizeof(int*));
    for(int i=0;i<n;i++){
        (to->t)[i] = malloc(m*sizeof(int));
    }
    to->vis = malloc(n*sizeof(int*));
    for(int i=0;i<n;i++){
        (to->vis)[i] = malloc(m*sizeof(int));
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            (to->t)[i][j]=(to->vis[i][j])=0;
        }
    }
    to->dx = malloc(4*sizeof(int));
    to->dy = malloc(4*sizeof(int));
    to->dx[0]=to->dx[1]=to->dy[2]=to->dy[3]=0;
    to->dx[2]=to->dy[0]=1;
    to->dx[3]=to->dy[1]=-1;
    return to;
}

int ingrid(int i, int j , grid* g){
    if(i<0 || j<0 || i>=g->n || j>=g->m || (g->vis[i][j]==1))return 0;
    return 1;
}

void  generate_grid(grid * g, int i, int j){
    //printf("%d %d\n",i,j);
    if(g->vis[i][j]){
        return ;
    }
    g->vis[i][j]=1;
    int k =  rand()%4;
    for(int o=0;o<4;o++){
        int ni = i + g->dx[(o+k)%4];
        int nj = j + g->dy[(o+k)%4];
        if(ingrid(ni,nj,g)){
            int u = (o+k)%4;
            g->t[i][j]+=(1<<u);
            g->t[ni][nj]+=1<<(u^1);
            generate_grid(g,ni,nj);
        }
    }
}


void show(grid * g){
    int n = g->n;
    int m = g->m;
    char gridc[2*n+1][2*m+1];
    int curi=0;
    int curj=0;
    gridc[curi][curj++]='-';
    for(int i=0;i<2*m-1;i++){
            gridc[curi][curj++]='-';
    }
    gridc[curi][curj++]='-';

    for(int i=0;i<n;i++){
            //printf("%d\n",i);
            curi++;
            curj=0;
        for(int j=0;j<m;j++){
            if(j==0){
                gridc[curi][curj++]='|';
            }
            gridc[curi][curj++]=' ';
            if(j==m-1){
                gridc[curi][curj++]='|';
            }else{
                if((g->t[i][j]&1)==0){
                    gridc[curi][curj++]='|';
                }else{
                    gridc[curi][curj++]=' ';
                }
            }
        }
        curj=0;
        curi++;
        for(int j=0;j<m;j++){
            if(j==0){
                if((g->t[i][j]&(1<<2))==0){
                    gridc[curi][curj++]='+';
                }else{
                   gridc[curi][curj++]='|';
                }
            }
            if((g->t[i][j]&(1<<2))==0){
                gridc[curi][curj++]='-';
            }else{
                gridc[curi][curj++]=' ';
            }
            if(j==m-1){
                if((g->t[i][j]&(1<<2))==0){
                    gridc[curi][curj++]='+';
                }else{
                    gridc[curi][curj++]='|';
                }
            }else{
                if((g->t[i][j]&(5))==0){
                    gridc[curi][curj++]='+';
                }else if((g->t[i][j]&(1))==0){
                    gridc[curi][curj++]='|';
                }else if((g->t[i][j]&(4))==0){
                    gridc[curi][curj++]='-';
                }else{
                    gridc[curi][curj++]=' ' ;
                }
            }
        }
    }
    for(int i=0;i<2*n+1;i++){
        for(int j=0;j<2*m+1;j++){
            int cond1 =0;
            int cond2=0;
            int cond3=0;
            int cond4=0;
            cond1 = (j>0 && (gridc[i][j-1]=='-' || gridc[i][j-1] == '+'));
            cond3 = (j<2*m && (gridc[i][j+1]=='-' || gridc[i][j+1]=='+'));
            cond4 = (i<2*n && (gridc[i+1][j]=='|' || gridc[i+1][j]=='+'));
            cond2 = (i>0 && (gridc[i-1][j]=='|' || gridc[i-1][j]=='+')) ;
            if((cond1+cond3) && (cond2+cond4)){
                gridc[i][j]='+';
            }
        }
    }
    for(int i=0;i<2*n+1;i++){
        for(int j=0;j<2*m+1;j++){
            printf("%c",gridc[i][j]);
        }
        printf("\n");
    }
}
