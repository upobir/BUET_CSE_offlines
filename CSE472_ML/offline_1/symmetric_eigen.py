import numpy as np

def random_invertible_symmatrix(n):
    while True:
        M = np.random.randint(-10, 10, size=(n, n))
        M = np.tril(M) + np.tril(M, -1).T
        if np.linalg.det(M) != 0:
            return M


def symdiagonalize(M):
    eigen_values, eigen_vectors = np.linalg.eigh(M)
    return eigen_vectors, np.diag(eigen_values), eigen_vectors.conj().T

if __name__ == "__main__":
    n = int(input("Enter dimension: "))

    M = random_invertible_symmatrix(n)
    print(M)

    A, B, C = symdiagonalize(M)
    print("diagnolized 3 matrices: ")
    print(A)
    print(B)
    print(C)

    M2 = A @ B @ C

    print("Is the diagnolization correct: ", np.allclose(M, M2))