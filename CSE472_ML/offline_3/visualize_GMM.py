import numpy as np
from gaussian_mixture_model import GaussianMixture
import matplotlib.pyplot as plt
import sys
from sklearn.decomposition import PCA

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python visualize_GMM.py <data_file> <no_of_clusters>")
        sys.exit(1)

    X = np.loadtxt(sys.argv[1])
    k = int(sys.argv[2])

    if X.shape[1] != 2:
        print("Non 2D data")
        pca = PCA(n_components=2)
        pca.fit(X)
        X = pca.transform(X)
        print("Finished PCA")

    params = {"n_component" : k}
    clusterer = GaussianMixture(params)
    clusterer.fit(X, True)

    x = np.linspace(X[:, 0].min(), X[:, 0].max(), 100)
    y = np.linspace(X[:, 1].min(), X[:, 1].max(), 100)

    xmesh, ymesh = np.meshgrid(x, y)

    c = ['b', 'g', 'r', 'c', 'm', 'y', 'k', 'w', 'p', 'o']
        
    for i, (means, covs, priors) in enumerate(clusterer.history):
        plt.clf()

        plt.scatter(X[:, 0], X[:, 1], s=1)
        plt.title("Iteration {}".format(i))


        for j in range(k):
            mean = means[j]
            cov = covs[j]

            Z = clusterer._multivariate_gaussian(np.array([xmesh.ravel(), ymesh.ravel()]).T, mean, cov)
            Z = Z.reshape(xmesh.shape)
            plt.contour(xmesh, ymesh, Z, 4, colors = c[j])

        plt.pause(0.08)

    plt.show()
