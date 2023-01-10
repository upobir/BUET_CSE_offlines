"""
main code that you will run
"""

import numpy as np
from gaussian_mixture_model import GaussianMixture
import matplotlib.pyplot as plt
import sys

if __name__ == '__main__':
    X = np.loadtxt(sys.argv[1])
    if len(X.shape) == 1:
        X = X.reshape((-1, 1))

    values = []
    for k in range(2, 11):
        params = {"n_component" : k}
        clusterer = GaussianMixture(params)
        clusterer.fit(X)
        log_likelihood = clusterer.score(X)
        values.append((k, log_likelihood))
        print(k, '->', log_likelihood)

    plt.plot([k for k, _ in values], [log_likelihood for _, log_likelihood in values])
    plt.xlabel("k")
    plt.ylabel("log likelihood")
    plt.show()

    k = max(values, key=lambda x: x[1])[0]
    print("number of components:", k)


