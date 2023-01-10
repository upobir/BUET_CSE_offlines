import numpy as np

class GaussianMixture:
    def __init__(self, params):
        """
        :param params:
        """
        self.k = params.get('n_component')
        self.max_iter = params.get('max_iter', 100)
        # self.eps = params.get('eps', 0.00000001)
        self.means = None
        self.covs = None
        self.priors = None

    def fit(self, X):
        """
        :param X:
        :return: self
        """
        assert len(X.shape) == 2

        n = X.shape[0]
        m = X.shape[1]

        y = np.random.randint(self.k, size=n)

        self.means = np.zeros((self.k, m,))
        self.covs = np.zeros((self.k, m, m,))
        self.priors = np.zeros((self.k,))

        for j in range(self.k):
            X_j = X[y == j]
            self.means[j] = np.mean(X_j, axis=0)
            self.covs[j] = np.cov(X_j.T)
            self.priors[j] = X_j.shape[0] / n

        # print('means', self.means)
        # print('covs', self.covs)
        # print('priors', self.priors)

        for iter in range(self.max_iter):
            responsibilities = self.predict_proba(X)

            N = np.sum(responsibilities, axis=0)

            self.means = np.dot(responsibilities.T, X) / N.reshape((-1, 1))

            for j in range(self.k):
                cov = np.zeros((m, m))
                for i in range(n):
                    cov += responsibilities[i, j] * np.outer(X[i] - self.means[j], X[i] - self.means[j])
                self.covs[j] = cov / N[j]

            self.priors = N / n

            # log_likelihood = np.sum(np.log(np.sum(responsibilities, axis=1)))
            # print('iter', _, log_likelihood)
            # if last is not None and abs(log_likelihood - last) < self.eps:
            #     break
            # last = log_likelihood

            # print('means', self.means)
            # print('covs', self.covs)
            # print('priors', self.priors)
            # print('------------------', iter)

        # print('means', self.means)
        # print('covs', self.covs)
        # print('priors', self.priors)

        return self

    def predict(self, X):
        """
        :param X:
        :return:
        """

        responsibilities = self.predict_proba(X)
        return np.argmax(responsibilities, axis=1)

    def predict_proba(self, X):
        """
        :param X:
        :return:
        """
        assert len(X.shape) == 2

        n = X.shape[0]
        m = X.shape[1]

        assert self.means.shape[1] == m

        responsibilities = np.zeros((n, self.k))
        for j in range(self.k):
            covdet = np.linalg.det(self.covs[j])
            mean = self.means[j]
            covinv = np.linalg.inv(self.covs[j])

            for i in range(n):
                x = (X[i] - mean).reshape((-1, 1))
                y = np.exp(-0.5 * (x.T @ covinv @ x))
                gaussian = 1 / ((2 * np.pi) ** (m / 2) * covdet ** 0.5) * y
                responsibilities[i, j] = self.priors[j] * gaussian
        
        return responsibilities / np.sum(responsibilities, axis=1).reshape((-1, 1))

    def score(self, X):
        """
        :param X:
        :return:
        """
        return np.sum(np.log(np.sum(self.predict_proba(X), axis=1)))