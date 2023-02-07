import cv2
import numpy as np
import os
import csv
from progress_bar import ProgressBar
import matplotlib.pyplot as plt

def process_image(image, dilation_shape, image_shape):
    image = 255 - image
    kernel = np.ones(dilation_shape, np.uint8)
    
    image = image / 255.0
    image = cv2.resize(image, image_shape)
    image = cv2.dilate(image, kernel, iterations=1)

    image /= np.max(image)
    return image

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

            image = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)

            image = process_image(image, (2, 2), image_shape)

            images.append(image.reshape(image_shape + (1,)))
            encoding = np.zeros(10)
            encoding[digit] = 1

            labels.append(encoding)
        
        bar.update(len(images) / read_count)

        if len(images) >= read_count:
            break

    images_cnt = len(images)
    print("Loaded {} images".format(images_cnt))

    return np.array(images), np.array(labels)

def save_image_data(X, y, image_folder, csv_path, prefix):
    if not os.path.exists(image_folder):
        os.makedirs(image_folder)

    bar = ProgressBar(50)

    with open(csv_path, mode='w') as csv_file:
        csv_writer = csv.writer(csv_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        csv_writer.writerow(["filename", "digit"])

        save_cnt = 0

        for i in range(X.shape[0]):
            number = str(i+1)
            number = "0" * (5 - len(number)) + number
            file_name = "{}_{}.png".format(prefix, number)
            file_path = os.path.join(image_folder, file_name)
            digit = np.argmax(y[i])

            csv_writer.writerow([file_name, digit])

            image = X[i].reshape(X[i].shape[:2])

            cv2.imwrite(file_path, 255.0 - image * 255.0)

            save_cnt += 1
            bar.update(save_cnt / X.shape[0])

    print("saved {} images".format(X.shape[0]))

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

    return X_train, y_train

def load_image_only(image_folder, image_shape, dilation_shape):
    images = []
    names = []

    for file_name in os.listdir(image_folder):
        if not file_name.endswith('.png'):
            continue

        file_path = os.path.join(image_folder, file_name)

        names.append(file_name)

        image = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)

        image = process_image(image, dilation_shape, image_shape)

        images.append(image.reshape(image_shape + (1,)))

    return names, np.array(images)