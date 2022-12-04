import numpy as np

def generate_random_matrix(n, m):
    return np.random.randint(-10, 10, size=(n, m))

def pseudo_inv(M):
    u, s, vh = np.linalg.svd(M, full_matrices=False)
    return vh.conj().T @ np.diag(1.0/s) @ u.conj().T

if __name__ == "__main__":
    n, m = map(int, input("Enter dimensions (separated by space): ").split())
    M = generate_random_matrix(n, m)
    print(M)

    Minv = np.linalg.pinv(M)
    Minv2 = pseudo_inv(M)

    print("Pseduo inverse (via np): ")
    print(Minv)
    print("Pseduo inverse (via formula): ")
    print(Minv2)

    print("are the inverses close: ", np.allclose(Minv, Minv2))