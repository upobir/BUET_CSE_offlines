import numpy as np
import matplotlib.pyplot as plt
import pickle
import sys
import os
import datetime

from data_handler import load_image_data, strat_split
from CNN import CNN
from ConvolutionLayer import ConvolutionLayer
from MaxPoolingLayer import MaxPoolingLayer
from ReLUActivationLayer import ReLUActivationLayer
from FlatteningLayer import FlatteningLayer
from DenseLayer import DenseLayer
from SoftMaxLayer import SoftMaxLayer
from sklearn.metrics import accuracy_score, log_loss, confusion_matrix, f1_score


if __name__ == '__main__':
    assert len(sys.argv) > 2, "Usage: python3 1705076_train.py <load|save|retrain> <model_file>"

    image_folder_format = "NumtaDB_with_aug/{}"
    csv_path_format = "NumtaDB_with_aug/{}.csv"
    train_set_size = 1000

    if sys.argv[1] == 'save' or sys.argv[1] == 'retrain':
        s = input("Enter train set size (default 1000): ")
        train_set_size = 1000 if s == '' else int(s)

        s = input("Enter learning rate (default 0.001): ")
        learning_rate = 0.001 if s == '' else float(s)

        s = input("Enter epochs (default 50): ")
        epochs = 50 if s == '' else int(s)

    Xa, ya = load_image_data(image_folder_format.format("training-a"), csv_path_format.format("training-a"), (64, 64), max_cnt = train_set_size + 500)
    Xb, yb = load_image_data(image_folder_format.format("training-b"), csv_path_format.format("training-b"), (64, 64), max_cnt = train_set_size + 500)
    Xc, yc = load_image_data(image_folder_format.format("training-c"), csv_path_format.format("training-c"), (64, 64), max_cnt = 3500)
    # Xd, yd = load_image_data(image_folder_format.format("training-d"), csv_path_format.format("training-d"), (64, 64), max_cnt = 1000)

    X_train = np.concatenate((Xa, Xb), axis=0)
    y_train = np.concatenate((ya, yb), axis=0)

    X_train, y_train = strat_split(X_train, y_train, train_set_size)

    X_validation = Xc
    y_validation = yc
    idx = np.random.permutation(X_validation.shape[0])
    X_validation = X_validation[idx][:3000]
    y_validation = y_validation[idx][:3000]

    # X_test = Xd
    # y_test = yd

    # shuffle training data
    # idx = np.random.permutation(X_train.shape[0])
    # X_train = X_train[idx]
    # y_train = y_train[idx]

    # for i in range(5):
    #     print(np.argmax(y_train[i]))
    #     plt.imshow(X_train[i], cmap='gray')
    #     plt.show()

    print("X_train.shape: {}".format(X_train.shape))
    print("y_train.shape: {}".format(y_train.shape))
    print("X_validation.shape: {}".format(X_validation.shape))
    print("y_validation.shape: {}".format(y_validation.shape))

    cnn = None
    filename = None

    if sys.argv[1] == 'load' or sys.argv[1] == 'retrain':
        filename = sys.argv[2] + '.pkl'
        assert os.path.exists(filename), "File not found: {}".format(filename)
        with open(filename, 'rb') as f:
            cnn = pickle.load(f)
    else:
        cnn = CNN(epochs=50, learning_rate=0.001, batch_size=64)

        cnn.addLayer(ConvolutionLayer(10, (4, 4), (2, 2), 0))
        cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
        cnn.addLayer(ReLUActivationLayer())
        cnn.addLayer(ConvolutionLayer(5, (3, 3), (2, 2), 1))
        cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
        cnn.addLayer(ReLUActivationLayer())
        cnn.addLayer(ConvolutionLayer(3, (3, 3), (2, 2), 1))
        cnn.addLayer(ReLUActivationLayer())
        cnn.addLayer(FlatteningLayer())
        cnn.addLayer(DenseLayer(35))
        cnn.addLayer(ReLUActivationLayer())
        cnn.addLayer(DenseLayer(10))
        cnn.addLayer(SoftMaxLayer())
        
        print("layer shapes")
        cnn.printShapes(X_train[:1])

    if sys.argv[1] == 'retrain' or sys.argv[1] == 'save':
        cnn.epochs = epochs
        cnn.learning_rate = learning_rate

        print()
        print("    training started at " + str(datetime.datetime.now()))
        print()

        cnn.train(X_train, y_train, X_validation, y_validation)

        print()
        print("    training ended at " + str(datetime.datetime.now()))
        print()

    y_pred = cnn.predict_proba(X_validation, batch_size=64)

    loss = log_loss(y_validation, y_pred)
    acc = accuracy_score(np.argmax(y_validation, axis=1), np.argmax(y_pred, axis=1))
    macrof1 = f1_score(np.argmax(y_validation, axis=1), np.argmax(y_pred, axis=1), average='macro')

    print("Validation loss: {}".format(loss))
    print("Validation accuracy: {}".format(acc))
    print("Validation macro f1: {}".format(macrof1))

    cm = confusion_matrix(np.argmax(y_validation, axis=1), np.argmax(y_pred, axis=1))

    plt.imshow(cm, interpolation='nearest', cmap=plt.cm.Blues)
    plt.title("Confusion Matrix")
    plt.colorbar()

    tick_marks = np.arange(10)
    plt.xticks(tick_marks, range(10), rotation=45)
    plt.yticks(tick_marks, range(10))

    plt.show()

    if sys.argv[1] == 'save' or sys.argv[1] == 'retrain':
        filename = sys.argv[2] + '.pkl'
        pickle.dump(cnn, open(filename, 'wb'))
        print("model saved to {}".format(filename))
    

