import numpy as np

# formulas
# P(X|y) = N(X|μ,Σ)
# P(y) = π
# P(y|X) = P(X|y)P(y) / sum(P(X|y)P(y))  target
# log_likelihood = sum(log(sum(P(X|y)P(y))))
# N(X|μ,Σ) = 1 / ((2π)^(m/2) * det(Σ)^(1/2)) * exp(-1/2 * (X-μ)^T * Σ^-1 * (X-μ))
# Nk = sum(P(y=k|X))
# πk = Nk / N
# μk = 1/Nk * sum(P(y=k|X) * X)
# Σk = 1/Nk * sum(P(y=k|X) * (X-μk) * (X-μk)^T)

class GaussianMixture:
    def __init__(self, params):
        self.k = params.get('n_component')
        self.max_iter = params.get('max_iter', 100)
        self.eps = params.get('eps', 0.001)
        self.trials = params.get('trials', 1)
        
        self.means = None
        self.covs = None
        self.priors = None
        self.history = []

    def _multivariate_gaussian(self, x, means, cov, prior = None):
        det = np.linalg.det(cov)

        assert det > 0, str(det) + ' ' + str(prior)
        denom = np.power(2 * np.pi, x.shape[1] / 2) * np.power(det, 0.5)

        e = np.exp( -0.5 * np.sum(((x-means) @ np.linalg.inv(cov)) * (x - means), axis=1))
        return 1.0 / denom * e

    def fit(self, X, with_history = False):
        assert len(X.shape) == 2

        n = X.shape[0]
        m = X.shape[1]

        self.history = None
        best_likelihood = None
        best_means = None
        best_covs = None
        best_priors = None
        best_history = None

        for trial in range(self.trials):
            log_likelihood = self._fit_once(X, with_history)

            if best_likelihood is None or log_likelihood > best_likelihood:
                best_means = self.means.copy()
                best_covs = self.covs.copy()
                best_priors = self.priors.copy()

                if with_history:
                    best_history = self.history.copy()

                best_likelihood = log_likelihood

        self.means = best_means.copy()
        self.covs = best_covs.copy()
        self.priors = best_priors.copy()
        if with_history:
            self.history = best_history.copy()

        return self

    def _fit_once(self, X, with_history = False):
        n = X.shape[0]
        m = X.shape[1]

        self.means = np.random.rand(self.k, m) + np.mean(X, axis=0) # (np.max(X, axis=0) - np.min(X, axis=0)) + np.min(X, axis=0)
        self.covs = np.zeros((self.k, m, m))
        for j in range(self.k):
            self.covs[j] = np.identity(m)
        self.priors = np.random.rand(self.k)
        self.priors /= np.sum(self.priors)        

        last_log_likelihood = None

        if with_history:
            self.history = [[self.means.copy(), self.covs.copy(), self.priors.copy()]]

        for iter in range(self.max_iter):
            # E step
            responsibilities = self.predict_proba(X)

            responsibilities += 1e-6
            responsibilities / np.sum(responsibilities, axis=1).reshape((-1, 1))

            # M step
            sizes = np.sum(responsibilities, axis=0)

            self.priors = sizes / n

            self.means = (responsibilities.T @ X) / sizes.reshape((-1, 1))

            for j in range(self.k):
                self.covs[j] = (responsibilities[:, j] * (X - self.means[j]).T) @ (X - self.means[j])
            self.covs /= sizes.reshape((-1, 1, 1))

            if with_history:
                self.history.append([self.means.copy(), self.covs.copy(), self.priors.copy()])

            # log likelihood
            log_likelihood = self.score(X)

            if last_log_likelihood is not None and abs(log_likelihood - last_log_likelihood) < self.eps:
                break

            last_log_likelihood = log_likelihood

        return log_likelihood

    def predict(self, X):
        responsibilities = self.predict_proba(X)
        return np.argmax(responsibilities, axis=1)

    def predict_proba(self, X):
        assert len(X.shape) == 2

        n = X.shape[0]
        m = X.shape[1]

        assert self.means.shape[1] == m

        responsibilities = np.zeros((self.k, n))
        for j in range(self.k):
            responsibilities[j] = self.priors[j] * self._multivariate_gaussian(X, self.means[j], self.covs[j], self.priors[j])
        responsibilities = responsibilities.T

        sum = np.sum(responsibilities, axis=1).reshape((-1, 1))
        sum[sum == 0] = 1
        
        return responsibilities / sum

    def score(self, X):
        assert len(X.shape) == 2

        n = X.shape[0]
        m = X.shape[1]

        assert self.means.shape[1] == m

        log_likelihood = np.zeros(n)
        for j in range(self.k):
            log_likelihood += self.priors[j] * self._multivariate_gaussian(X, self.means[j], self.covs[j], self.priors[j])
        log_likelihood = np.sum(np.log(log_likelihood))
        return log_likelihood