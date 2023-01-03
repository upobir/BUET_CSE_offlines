import numpy as np
import pandas as pd

def load_dataset(path = 'data_banknote_authentication.csv'):
    """
    function for reading data from csv
    and processing to return a 2D feature matrix and a vector of class
    :return:
    """
    df = df = pd.read_csv(path)
    # col_count = len(df.columns)
    # row_count = len(df)
    X = df[df.columns[:-1]].to_numpy()
    y = df[df.columns[-1]].to_numpy()

    return X, y


def split_dataset(X, y, test_size, shuffle):
    """
    function for spliting dataset into train and test
    :param X:
    :param y:
    :param float test_size: the proportion of the dataset to include in the test split
    :param bool shuffle: whether to shuffle the data before splitting
    :return:
    """
    assert len(X) == len(y)
    row_count = len(X)

    if shuffle:
        perm = np.random.permutation(row_count)
        X = X[perm]
        y = y[perm]

    test_row_count = int(test_size * row_count)

    train_mask = np.ones(row_count)
    train_mask[:test_row_count] = 0
    np.random.shuffle(train_mask)

    X_train = X[train_mask == 1]
    y_train = y[train_mask == 1]
    X_test = X[train_mask == 0]
    y_test = y[train_mask == 0]
    return X_train, y_train, X_test, y_test


def bagging_sampler(X, y):
    """
    Randomly sample with replacement
    Size of sample will be same as input data
    :param X:
    :param y:
    :return:
    """
    assert X.shape[0] == y.shape[0]

    row_count = X.shape[0]

    sample = np.random.choice(row_count, row_count)
    X_sample, y_sample = X[sample], y[sample]
    
    assert X_sample.shape == X.shape
    assert y_sample.shape == y.shape
    return X_sample, y_sample
