import cv2
import numpy as np
import os
import csv
from progress_bar import ProgressBar

def load_image_data(image_folder, csv_path, image_shape, max_cnt = None):


    mapping = {}
    with open(csv_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0

        for row in csv_reader:
            if line_count == 0:
                filename_index = row.index("filename")
                digit_index = row.index("digit")
            else:
                filename = row[filename_index]
                digit = int(row[digit_index])
                mapping[filename] = digit

            line_count += 1

    read_count = min(max_cnt, len(mapping)) if max_cnt is not None else len(mapping)

    images = []
    labels = []

    bar = ProgressBar(50)

    for file_name in os.listdir(image_folder):
        file_path = os.path.join(image_folder, file_name)
        if file_name in mapping:
            digit = mapping[file_name]

            image = cv2.imread(file_path, cv2.IMREAD_COLOR)
            image = cv2.resize(image, image_shape)
            # image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
            image = image / 255.0

            images.append(image)
            # create one-hot encoding from digit value with range
            encoding = np.zeros(10)
            encoding[digit] = 1

            labels.append(encoding)
        
        bar.update(len(images) / read_count)

        if len(images) >= read_count:
            break

    images_cnt = len(images)
    print("Loaded {} images".format(images_cnt))

    return np.array(images), np.array(labels)

def strat_split(X, y, sz):
    indiv_sz = sz // y.shape[1]

    for i in range(y.shape[1]):
        idx = np.where(y[:, i] == 1)[0]
        np.random.shuffle(idx)
        idx = idx[:indiv_sz]

        if i == 0:
            train_idx = idx
        else:
            train_idx = np.concatenate((train_idx, idx))

    X_train = X[train_idx]
    y_train = y[train_idx]

    print(y_train.sum(axis=0))

    return X_train, y_train