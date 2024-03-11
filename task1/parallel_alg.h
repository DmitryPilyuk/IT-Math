#ifndef IT_MATH_PARALLEL_ALG_H
#define IT_MATH_PARALLEL_ALG_H

#include <math.h>
#include <omp.h>
#include "grid.h"

#define BLOCK_SIZE 64

size_t compute_parallel(d_grid *grid);

#endif // IT_MATH_PARALLEL_ALG_H
