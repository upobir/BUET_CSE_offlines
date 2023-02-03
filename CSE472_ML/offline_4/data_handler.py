import cv2
import numpy as np
import os
import csv

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


    images = []
    labels = []
    for file_name in os.listdir(image_folder):
        file_path = os.path.join(image_folder, file_name)
        if file_name in mapping:
            digit = mapping[file_name]

            image = cv2.imread(file_path, cv2.IMREAD_COLOR)
            image = cv2.resize(image, image_shape)
            # image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

            images.append(image)
            labels.append(digit)

        if max_cnt is not None and len(images) >= max_cnt:
            break

    images_cnt = len(images)
    print("Loaded {} images".format(images_cnt))

    return np.array(images), np.array(labels)