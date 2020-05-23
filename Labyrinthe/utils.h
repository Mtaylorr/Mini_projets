#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

typedef struct grid{
    int n,m;
    int ** t;
    int ** vis;
    int *dx;
    int *dy;
}grid;

grid * getnewgrid(int n , int m);

int ingrid(int i, int j , grid* g);

void  generate_grid(grid * g, int i, int j);

void show(grid* g);


#endif // UTILS_H_INCLUDED
