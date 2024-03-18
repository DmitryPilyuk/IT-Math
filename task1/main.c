#include <stdio.h>
#include "util.h"
#include <math.h>

double u1(double x, double y) { return (1 - 2 * y) * (1 - 2 * x) * 100; }
double f1(double x, double y) { return 0; }

double u2(double x, double y) { return sin(x * y) / 2; }
double f2(double x, double y) { return -((x * x + y * y) * sin(x * y) / 2); }

int main()
{
    size_t size[] = {100, 500, 1000, 1500, 2000};
    int threads[] = {1, 4, 8, 16};
    f_R2_to_R f[] = {f1, f2};
    f_R2_to_R u[] = {u1, f1};
    size_t block_size = 32;
    size_t len_size = sizeof(size) / sizeof(size[0]);
    size_t len_f = sizeof(f) / sizeof(f[0]);
    size_t len_threads = sizeof(threads) / sizeof(threads[0]);
    double eps = 0.01;
    experiment_res res[len_size][len_threads];
    int n_of_experiments = 10;
    for (int k = 0; k < n_of_experiments; k++)
    {
        for (size_t t = 0; t < len_f; t++)
        {
            for (size_t i = 0; i < len_size; i++)
            {
                for (size_t j = 0; j < len_threads; j++)
                {
                    if (threads[j] == 1)
                    {
                        res[i][j] = run_sequential(f[t], u[t], size[i], eps);
                    }
                    else
                    {
                        res[i][j] = run_parallel(f[t], u[t], size[i], eps, threads[j], block_size);
                    }
                }
            }

            FILE *fpt;
            char filename[20];
            sprintf(filename, "f%lu_result%d.csv", (t + 1), (k + 1));
            fpt = fopen(filename, "w");
            fprintf(fpt, "Size");
            for (size_t i = 0; i < len_threads; i++)
            {
                fprintf(fpt, ", k_%d, t_%d", threads[i], threads[i]);
            }
            fprintf(fpt, "\n");
            for (size_t i = 0; i < len_size; i++)
            {
                fprintf(fpt, "%lu", size[i]);
                for (size_t j = 0; j < len_threads; j++)
                {
                    fprintf(fpt, ", %lu, %f", res[i][j].number_of_iterations, res[i][j].dt);
                }
                fprintf(fpt, "\n");
            }
            fclose(fpt);
        }
    }
    return 0;
}
