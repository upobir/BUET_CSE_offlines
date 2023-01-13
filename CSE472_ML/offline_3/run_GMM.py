import numpy as np
from gaussian_mixture_model import GaussianMixture
import matplotlib.pyplot as plt
import sys

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python run_GMM.py <data_file>")
        sys.exit(1)

    X = np.loadtxt(sys.argv[1])
    if len(X.shape) == 1:
        X = X.reshape((-1, 1))

    values = []
    for k in range(1, 11):
        params = {"n_component" : k, 'trials': 4}
        clusterer = GaussianMixture(params)
        clusterer.fit(X)
        log_likelihood = clusterer.score(X)
        values.append((k, log_likelihood))
        print(k, ' clusters, log likelihood = ', log_likelihood)

    
    for i in range(len(values)):
        v = values[i]
        converged = True
        for j in range(i+1, len(values)):
            if abs((v[1] - values[j][1]) / v[1]) > 0.01:
                converged = False
                break
        if converged:
            k_star = v[0]
            break

    print()
    print("expected k* = ", k_star)

    plt.plot([k for k, _ in values], [log_likelihood for _, log_likelihood in values])
    plt.xlabel("k")
    plt.ylabel("log likelihood")
    plt.xticks([k for k, _ in values])
    plt.show()

    