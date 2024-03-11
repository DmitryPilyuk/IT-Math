#include <stdio.h>
#include "util.h"
#include <math.h>

double u1(double x, double y) { return (1 - 2 * y) * (1 - 2 * x) * 100; }
double f1(double x, double y) { return 0; }

int main()
{
    size_t size[] = {100, 200, 400, 600, 800, 1000, 1500, 2000};
    int threads[] = {1, 4, 8, 16};
    size_t len_size = sizeof(size) / sizeof(size[0]);
    size_t len_threads = sizeof(threads) / sizeof(threads[0]);
    double eps = 0.01;
    experiment_res res[len_size][len_threads];
    double ts[len_size][len_threads];
    size_t iters[len_size][len_threads];
    int n_of_experiments = 5;
    for (size_t i = 0; i < len_size; i++)
    {
        for (size_t j = 0; j < len_threads; j++)
        {
            ts[i][j] = 0.0;
            iters[i][j] = 0;
        }
    }
    for (int k = 0; k < n_of_experiments; k++)
    {
        for (size_t i = 0; i < len_size; i++)
        {
            for (size_t j = 0; j < len_threads; j++)
            {
                if (threads[j] == 1)
                {
                    res[i][j] = run_sequential(f1, u1, size[i], eps);
                }
                else
                {
                    res[i][j] = run_parallel(f1, u1, size[i], eps, threads[j]);
                }
            }
        }

        for (size_t i = 0; i < len_size; i++)
        {
            for (size_t j = 0; j < len_threads; j++)
            {
                ts[i][j] += res[i][j].dt;
                iters[i][j] += res[i][j].number_of_iterations;
            }
        }
    }
    for (size_t i = 0; i < len_size; i++)
    {
        for (size_t j = 0; j < len_threads; j++)
        {
            ts[i][j] = ts[i][j] / n_of_experiments;
            iters[i][j] = iters[i][j] / n_of_experiments;
        }
    }

    FILE *fpt;
    fpt = fopen("result.csv", "w");
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
            fprintf(fpt, ", %lu, %f", iters[i][j], ts[i][j]);
        }
        fprintf(fpt, "\n");
    }
    return 0;
}
