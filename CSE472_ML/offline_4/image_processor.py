import numpy as np
import matplotlib.pyplot as plt
import pickle
import sys
import os
import datetime

from data_handler import load_image_data, save_image_data

if __name__ == '__main__':
    image_folder_format = "NumtaDB_with_aug/{}"
    csv_path_format = "NumtaDB_with_aug/{}.csv"

    max_size = 10000

    Xa, ya = load_image_data(image_folder_format.format("training-a"), csv_path_format.format("training-a"), (128, 128), max_cnt = max_size)
    Xb, yb = load_image_data(image_folder_format.format("training-b"), csv_path_format.format("training-b"), (128, 128), max_cnt = max_size)
    Xc, yc = load_image_data(image_folder_format.format("training-c"), csv_path_format.format("training-c"), (128, 128), max_cnt = max_size)
    Xd, yd = load_image_data(image_folder_format.format("training-d"), csv_path_format.format("training-d"), (128, 128), max_cnt = max_size)

    save_folder_format = "converted_data/{}"
    save_csv_path_format = "converted_data/{}.csv"
    save_image_data(Xa, ya, save_folder_format.format("training-a"), save_csv_path_format.format("training-a"), 'a')
    save_image_data(Xb, yb, save_folder_format.format("training-b"), save_csv_path_format.format("training-b"), 'b')
    save_image_data(Xc, yc, save_folder_format.format("training-c"), save_csv_path_format.format("training-c"), 'c')
    save_image_data(Xd, yd, save_folder_format.format("training-d"), save_csv_path_format.format("training-d"), 'd')