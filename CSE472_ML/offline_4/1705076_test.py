import sys
import pickle
import csv
import os

from data_handler import load_image_only
from CNN import CNN

if __name__ == "__main__":
    assert len(sys.argv) == 2

    path_to_folder = sys.argv[1]

    names, images = load_image_only(path_to_folder, (64, 64), (2, 2))

    print("images loaded")

    cnn = pickle.load(open("1705076.pkl", "rb"))

    predictions = cnn.predict(images)

    print("prediciton completed")

    csv_file = os.path.join(path_to_folder, "1705076_predictions.csv")

    with open(csv_file, 'w', newline='') as f:
        writer = csv.writer(f)

        writer.writerow(["FileName", "Digit"])

        for i in range(len(names)):
            writer.writerow([names[i], predictions[i]])

    print("predictions saved")