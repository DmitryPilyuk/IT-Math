from random import normalvariate

from numpy.linalg import norm

import util
from math import sqrt
import numpy as np

EPS = 0.01


def numpy_svd(matrix: np.matrix, k):
    u, s, vh = np.linalg.svd(matrix, full_matrices=False)
    u = u[:, :k]
    s = s[:k]
    vh = vh[:k, :]
    return util.SVD(u, s, vh)


def random_unit_vector(n):
    unnormalized = [normalvariate(0, 1) for _ in range(n)]
    theNorm = sqrt(sum(x * x for x in unnormalized))
    return [x / theNorm for x in unnormalized]


def svd_1d(A, epsilon):
    ''' The one-dimensional SVD '''

    n, m = A.shape
    x = random_unit_vector(min(n, m))
    lastV = None
    currentV = x

    if n > m:
        B = np.dot(A.T, A)
    else:
        B = np.dot(A, A.T)

    iterations = 0
    while True:
        iterations += 1
        lastV = currentV
        currentV = np.dot(B, lastV)
        currentV = currentV / norm(currentV)

        if abs(np.dot(currentV, lastV)) > 1 - epsilon:
            return currentV


def power_svd(A, k):
    A = np.array(A, dtype=float)
    n, m = A.shape
    svdSoFar = []
    if k is None:
        k = min(n, m)

    for i in range(k):
        matrixFor1D = A.copy()

        for singularValue, u, v in svdSoFar[:i]:
            matrixFor1D -= singularValue * np.outer(u, v)

        if n > m:
            v = svd_1d(matrixFor1D, epsilon=EPS)  # next singular vector
            u_unnormalized = np.dot(A, v)
            sigma = norm(u_unnormalized)  # next singular value
            u = u_unnormalized / sigma
        else:
            u = svd_1d(matrixFor1D, epsilon=EPS)  # next singular vector
            v_unnormalized = np.dot(A.T, u)
            sigma = norm(v_unnormalized)  # next singular value
            v = v_unnormalized / sigma

        svdSoFar.append((sigma, u, v))

    s, u, vh = [np.array(x) for x in zip(*svdSoFar)]
    return util.SVD(np.matrix(u.T), s, vh)
