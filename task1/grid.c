#include "grid.h"

double **create_2d_arr(size_t size)
{
    double **arr = malloc(size * sizeof(double *));
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = malloc(size * sizeof(double));
    }
    return arr;
}

void free_2d_arr(double **arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(arr[i]);
        arr[i] = NULL;
    }
    free(arr);
    arr = NULL;
}

d_grid *create_grid(size_t size, f_R2_to_R f, f_R2_to_R g, double eps)
{
    d_grid *grid = malloc(sizeof(d_grid));
    grid->size = size;
    grid->eps = eps;
    grid->h = 1 / (double)(size - 1);
    grid->f = create_2d_arr(size);
    grid->u = create_2d_arr(size);
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
            {
                grid->u[i][j] = g(i * grid->h, j * grid->h);
            }
            else
            {
                grid->u[i][j] = 0;
            }
            grid->f[i][j] = f(i * grid->h, j * grid->h);
        }
    }
    return grid;
}

void free_grid(d_grid *grid)
{
    free_2d_arr(grid->f, grid->size);
    free_2d_arr(grid->u, grid->size);
    free(grid);
    grid = NULL;
}
