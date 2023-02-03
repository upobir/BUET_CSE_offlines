from data_handler import load_image_data
import numpy as np



if __name__ == '__main__':

    image_folder_format = "NumtaDB_with_aug/{}"
    csv_path_format = "NumtaDB_with_aug/{}.csv"

    Xa, ya = load_image_data(image_folder_format.format("training-a"), csv_path_format.format("training-a"), (32, 32), max_cnt = 100)
    Xb, yb = load_image_data(image_folder_format.format("training-b"), csv_path_format.format("training-b"), (32, 32), max_cnt = 100)
    Xc, yc = load_image_data(image_folder_format.format("training-c"), csv_path_format.format("training-c"), (32, 32), max_cnt = 100)
    Xd, yd = load_image_data(image_folder_format.format("training-d"), csv_path_format.format("training-d"), (32, 32), max_cnt = 100)

    # TODO data partition
    X_train = np.concatenate((Xa, Xb), axis=0)
    y_train = np.concatenate((ya, yb), axis=0)

    X_validation = Xc
    y_validation = yc

    X_test = Xd
    y_test = yd

    

