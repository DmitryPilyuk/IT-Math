#ifndef IT_MATH_GRID_H
#define IT_MATH_GRID_H

#include <stdlib.h>

typedef double (*f_R2_to_R)(double, double);

typedef struct d_grid
{
    double h;
    double eps;
    double **u;
    double **f;
    size_t size;
} d_grid;

d_grid *create_grid(size_t size, f_R2_to_R f, f_R2_to_R g, double eps);

void free_grid(d_grid *grid);

#endif // IT_MATH_GRID_H
