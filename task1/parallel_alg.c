#include "parallel_alg.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

double compute_block(d_grid *grid, size_t x, size_t y)
{
    size_t i0 = 1 + BLOCK_SIZE * x;
    size_t j0 = 1 + BLOCK_SIZE * y;
    double dm = 0;
    for (size_t i = i0; i < min(i0 + BLOCK_SIZE, grid->size - 1); i++)
    {
        for (size_t j = j0; j < min(j0 + BLOCK_SIZE, grid->size - 1); j++)
        {
            double temp = grid->u[i][j];
            grid->u[i][j] =
                0.25 * (grid->u[i - 1][j] + grid->u[i + 1][j] + grid->u[i][j - 1] + grid->u[i][j + 1] - grid->h * grid->h * grid->f[i][j]);
            double d = fabs(temp - grid->u[i][j]);
            if (dm < d)
            {
                dm = d;
            }
        }
    }
    return dm;
}

size_t compute_parallel(d_grid *grid)
{
    size_t sz = grid->size - 2;
    int number_of_blocks = sz / BLOCK_SIZE + (sz % BLOCK_SIZE != 0 ? 1 : 0);
    size_t k = 0;
    double d_max;
    double *dm = calloc(number_of_blocks, sizeof(double));
    do
    {
        k++;
        d_max = 0;
        for (int nx = 0; nx < number_of_blocks; nx++)
        {
            dm[nx] = 0;
            int i, j;
            double d;

#pragma omp parallel for shared(grid, nx, dm) private(i, j, d)
            for (i = 0; i < nx + 1; i++)
            {
                j = nx - i;
                d = compute_block(grid, i, j);
                if (dm[i] < d)
                {
                    dm[i] = d;
                }
            }
        }
        for (int nx = number_of_blocks - 2; nx >= 0; nx--)
        {
            int i, j;
            double d;
#pragma omp parallel for shared(grid, nx, dm) private(i, j, d)
            for (i = number_of_blocks - nx - 1; i < number_of_blocks; i++)
            {
                j = number_of_blocks + ((number_of_blocks - 2) - nx) - i;
                d = compute_block(grid, i, j);
                if (dm[i] < d)
                {
                    dm[i] = d;
                }
            }
        }
        for (int i = 0; i < number_of_blocks; i++)
        {
            if (d_max < dm[i])
                d_max = dm[i];
        }
    } while (d_max > grid->eps);
    free(dm);
    return k;
}
