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
from metrics import cross_entropy


if __name__ == '__main__':
    image_folder_format = "NumtaDB_with_aug/{}"
    csv_path_format = "NumtaDB_with_aug/{}.csv"

    Xa, ya = load_image_data(image_folder_format.format("training-a"), csv_path_format.format("training-a"), (128, 128), max_cnt = 2000)
    Xb, yb = load_image_data(image_folder_format.format("training-b"), csv_path_format.format("training-b"), (128, 128), max_cnt = 2000)
    Xc, yc = load_image_data(image_folder_format.format("training-c"), csv_path_format.format("training-c"), (128, 128), max_cnt = 1000)
    Xd, yd = load_image_data(image_folder_format.format("training-d"), csv_path_format.format("training-d"), (128, 128), max_cnt = 1000)

    X_train = np.concatenate((Xa, Xb), axis=0)
    y_train = np.concatenate((ya, yb), axis=0)

    X_train, y_train = strat_split(X_train, y_train, 1500)

    X_validation = Xc
    y_validation = yc

    X_test = Xd
    y_test = yd

    # TODO do stratified splits for better dataset

    print("X_train.shape: {}".format(X_train.shape))
    print("y_train.shape: {}".format(y_train.shape))

    cnn = None
    filename = None

    if len(sys.argv) > 1:
        filename = sys.argv[1] + '.pkl'
        assert os.path.exists(filename), "File not found: {}".format(filename)
        with open(filename, 'rb') as f:
            cnn = pickle.load(f)
    else:
        cnn = CNN(epochs=100, learning_rate=0.0005, batch_size=64)

        cnn.addLayer(ConvolutionLayer(8, (4, 4), (1, 1), 0))
        cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
        cnn.addLayer(ConvolutionLayer(5, (3, 3), (1, 1), 0))
        cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
        cnn.addLayer(ReLUActivationLayer())
        cnn.addLayer(FlatteningLayer())
        cnn.addLayer(DenseLayer(20))
        cnn.addLayer(ReLUActivationLayer())
        cnn.addLayer(DenseLayer(30))
        cnn.addLayer(ReLUActivationLayer())
        cnn.addLayer(DenseLayer(10))
        cnn.addLayer(SoftMaxLayer())

        print()
        print("    training started at " + str(datetime.datetime.now()))
        print()

        cnn.train(X_train, y_train)

        print()
        print("    training ended at " + str(datetime.datetime.now()))
        print()

    y_pred = cnn.predict_proba(X_validation)

    loss = cross_entropy(y_validation, y_pred)

    print("Validation loss: {}".format(loss))

    # print("do you want to save the model? (y/n)")
    # if input() == 'y':
    #     filename = input("filename (without pkl): ")
    #     filename += '.pkl'
        
    #     pickle.dump(cnn, open(filename, 'wb'))

    pickle.dump(cnn, open("first_test.pkl", 'wb'))
    print("model saved to first_test.pkl")

