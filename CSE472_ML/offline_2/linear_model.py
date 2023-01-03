import numpy as np

def sigmoid(z):
    return 1.0/(1.0 + np.exp(-z))

class LogisticRegression:
    def __init__(self, params):
        """
        figure out necessary params to take as input
        :param params:
        """
        self.alpha = params.get('alpha', 0.3)
        self.max_iter = params.get('max_iter', 100)
        self.eps = params.get('eps', 0.0001)
        self.theta = None

    def fit(self, X, y):
        """
        :param X:
        :param y:
        :return: self
        """
        assert X.shape[0] == y.shape[0]
        assert len(X.shape) == 2

        row_count = X.shape[0]

        if self.theta is None:
            self.theta = np.zeros(X.shape[1]+1)

        iter = 0
        while True:
            if(iter >= self.max_iter):
                break

            h_theta = sigmoid(self.theta[0] + X.dot(self.theta[1:]))

            old_theta = self.theta.copy()

            self.theta[0] = self.theta[0] + self.alpha * (y - h_theta).sum() / row_count
            self.theta[1:] = self.theta[1:] + self.alpha * (X * (y - h_theta)[:, np.newaxis]).sum(axis=0) / row_count

            

            if np.abs(self.theta - old_theta).max() < self.eps:
                break

            iter += 1

        return


    def predict(self, X):
        """
        function for predicting labels of for all datapoint in X
        :param X:
        :return:
        """
        assert self.theta is not None
        assert len(X.shape) == 2
        assert X.shape[1] + 1 == len(self.theta)

        y = self.theta[0] + X.dot(self.theta[1:])
        y = sigmoid(y)

        return (y > 0.5).astype(int)