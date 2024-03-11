#include "sequential_alg.h"

size_t compute_sequential(d_grid *grid)
{
    size_t k = 0;
    double d_max;
    double **u = grid->u;
    do
    {
        d_max = 0;
        for (size_t i = 1; i < grid->size - 1; i++)
            for (size_t j = 1; j < grid->size - 1; j++)
            {
                double temp = u[i][j];
                u[i][j] = 0.25 * (u[i - 1][j] + u[i + 1][j] +
                                  u[i][j - 1] + u[i][j + 1] - grid->h * grid->h * grid->f[i][j]);
                double dm = fabs(temp - u[i][j]);
                if (d_max < dm)
                    d_max = dm;
            }
        k++;
    } while (d_max > grid->eps);
    return k;
}
