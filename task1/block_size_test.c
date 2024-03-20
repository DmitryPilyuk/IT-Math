#include <stdio.h>
#include "util.h"
#include <math.h>

double u1(double x, double y) { return (1 - 2 * y) * (1 - 2 * x) * 100; }
double f1(double x, double y) { return 0; }

int main()
{
    size_t size[] = {100, 500, 1000, 1500, 2000};
    int block_size[] = {1, 16, 32, 64, 128};
    size_t len_size = sizeof(size) / sizeof(size[0]);
    size_t len_block_size = sizeof(block_size) / sizeof(block_size[0]);
    int threads = 8;
    double eps = 0.01;
    experiment_res res[len_size][len_block_size];
    int n_of_experiments = 10;
    for (int k = 0; k < n_of_experiments; k++)
    {
        for (size_t i = 0; i < len_size; i++)
        {
            for (size_t j = 0; j < len_block_size; j++)
            {
                {
                    res[i][j] = run_parallel(f1, u1, size[i], eps, threads, block_size[j]);
                }
            }
        }
        FILE *fpt;
        char filename[24];
        sprintf(filename, "result_block_size%d.csv", (k + 1));
        fpt = fopen(filename, "w");
        fprintf(fpt, "Size");
        for (size_t i = 0; i < len_block_size; i++)
        {
            fprintf(fpt, ", k_%d, t_%d", block_size[i], block_size[i]);
        }
        fprintf(fpt, "\n");
        for (size_t i = 0; i < len_size; i++)
        {
            fprintf(fpt, "%lu", size[i]);
            for (size_t j = 0; j < len_block_size; j++)
            {
                fprintf(fpt, ", %lu, %f", res[i][j].number_of_iterations, res[i][j].dt);
            }
            fprintf(fpt, "\n");
        }
        fclose(fpt);
    }
    return 0;
}