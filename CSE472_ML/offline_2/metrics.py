"""
Refer to: https://en.wikipedia.org/wiki/Precision_and_recall#Definition_(classification_context)
"""


def accuracy(y_true, y_pred):
    """

    :param y_true:
    :param y_pred:
    :return:
    """

    assert(len(y_true) == len(y_pred))
    P = (y_true == 1).sum()
    N = (y_true == 0).sum()

    TP = ((y_pred == 1) & (y_true == y_pred)).sum()
    TN = ((y_pred == 0) & (y_true == y_pred)).sum()

    return (TP+TN)/(P+N)
    

def precision_score(y_true, y_pred):
    """

    :param y_true:
    :param y_pred:
    :return:
    """
    assert(len(y_true) == len(y_pred))
    TP = ((y_pred == 1) & (y_true == y_pred)).sum()
    PP = (y_pred == 1).sum()
    return TP/PP


def recall_score(y_true, y_pred):
    """

    :param y_true:
    :param y_pred:
    :return:
    """
    assert(len(y_true) == len(y_pred))
    P = (y_true == 1).sum()
    TP = ((y_pred == 1) & (y_true == y_pred)).sum()

    return TP/P


def F1_score(y_true, y_pred):
    """

    :param y_true:
    :param y_pred:
    :return:
    """
    assert(len(y_true) == len(y_pred))
    TP = ((y_pred == 1) & (y_true == y_pred)).sum()
    FP = ((y_pred == 1) & (y_true != y_pred)).sum()
    FN = ((y_pred == 0) & (y_true != y_pred)).sum()
    return (2*TP)/(2*TP+FP+FN)
