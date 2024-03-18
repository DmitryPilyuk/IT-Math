#ifndef IT_MATH_UTIL_H
#define IT_MATH_UTIL_H

#include "grid.h"
#include "parallel_alg.h"
#include "sequential_alg.h"
#include "omp.h"

typedef struct experiment_res
{
    double dt;
    size_t number_of_iterations;
    size_t size;
} experiment_res;

experiment_res run_sequential(f_R2_to_R f, f_R2_to_R g, size_t size, double eps);
experiment_res run_parallel(f_R2_to_R f, f_R2_to_R g, size_t size, double eps, int number_of_treads, size_t block_size);

#endif // IT_MATH_UTIL_H
