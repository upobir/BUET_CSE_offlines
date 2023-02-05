import numpy as np

def cross_entropy(y_true, y_pred):
    return -np.mean(y_true * np.log(y_pred))