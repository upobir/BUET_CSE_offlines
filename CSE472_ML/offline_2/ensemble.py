from data_handler import bagging_sampler
from copy import deepcopy
import numpy as np

class BaggingClassifier:
    def __init__(self, base_estimator, n_estimator):
        """
        :param base_estimator:
        :param n_estimator:
        :return:
        """
        self.estimators = [deepcopy(base_estimator) for _ in range(n_estimator)]

    def fit(self, X, y):
        """
        :param X:
        :param y:
        :return: self
        """
        assert X.shape[0] == y.shape[0]
        assert len(X.shape) == 2
        
        for estimator in self.estimators:
            X_sample, y_sample = bagging_sampler(X, y)
            # print(np.unique(X_sample, axis=0))
            estimator.fit(X_sample, y_sample)

        return

    def predict(self, X):
        """
        function for predicting labels of for all datapoint in X
        apply majority voting
        :param X:
        :return:
        """
        predictions = np.array([estimator.predict(X) for estimator in self.estimators])
        # return (predictions.sum(axis=0) > len(self.estimators)/2).astype(int)

        prediction = np.apply_along_axis(lambda x: np.bincount(x).argmax(), axis=0, arr=predictions)

        # for row in predictions.T:
        #     print(np.bincount(row))

        return prediction

