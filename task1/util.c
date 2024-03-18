#include "util.h"

experiment_res run_sequential(f_R2_to_R f, f_R2_to_R g, size_t size, double eps)
{
    double t_start, t_end;
    size_t number_of_iterations;
    d_grid *grid = create_grid(size, f, g, eps, 1);
    t_start = omp_get_wtime();
    number_of_iterations = compute_sequential(grid);
    t_end = omp_get_wtime();
    experiment_res res = {.dt = t_end - t_start, .number_of_iterations = number_of_iterations, .size = size};
    free_grid(grid);
    return res;
}

experiment_res run_parallel(f_R2_to_R f, f_R2_to_R g, size_t size, double eps, int number_of_treads, size_t block_size)
{
    double t_start, t_end;
    size_t number_of_iterations;
    d_grid *grid = create_grid(size, f, g, eps, block_size);
    omp_set_num_threads(number_of_treads);
    t_start = omp_get_wtime();
    number_of_iterations = compute_parallel(grid);
    t_end = omp_get_wtime();
    experiment_res res = {.dt = t_end - t_start, .number_of_iterations = number_of_iterations, .size = size};
    free_grid(grid);
    return res;
}
