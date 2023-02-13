import sys
import pickle
import csv
import os
import datetime

import cv2
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.pyplot as plt
from sklearn.metrics import accuracy_score, log_loss, f1_score, confusion_matrix

###############################################################################
########################## benchmark.py #######################################
###############################################################################

def benchmark(actual_encodings, predicted_probs):
    """
    benchmark model
    """
    print("Log loss: ", log_loss(actual_encodings, predicted_probs))
    actual_labels = np.argmax(actual_encodings, axis=1)
    predicted_labels = np.argmax(predicted_probs, axis=1)

    print("Accuracy: ", accuracy_score(actual_labels, predicted_labels))
    print("Macro F1: ", f1_score(actual_labels, predicted_labels, average='macro'))

    cm = confusion_matrix(actual_labels, predicted_labels)
    print("Confusion matrix: ")
    print(cm)

    fig, ax = plt.subplots()
    im = ax.imshow(cm, cmap='Blues')

    ax.set_xticks(np.arange(10))
    ax.set_yticks(np.arange(10))

    for i in range(10):
        for j in range(10):
            text = ax.text(j, i, cm[i, j], ha="center", va="center", color=("white" if cm[i, j] > 0.5 * np.max(cm) else "black"))

    ax.set_xlabel("Predicted")
    ax.set_ylabel("True")
    ax.set_title("Confusion matrix")

    fig.tight_layout()
    plt.show()

    return

###############################################################################
########################## MaxPoolingLayer.py #################################
###############################################################################

class MaxPoolingLayer:
    """
    Max pooling layer
    """
    def __init__(self, filter_size = (2, 2), stride = (1, 1)):
        self.filter_size = filter_size
        self.stride = stride


    def forward(self, inputs):
        """
        Forward pass
        """
        assert(len(inputs.shape) == 4)
        self.inputs = inputs
        output_shape = (
            inputs.shape[0],
            (inputs.shape[1] - self.filter_size[0]) // self.stride[0] + 1,
            (inputs.shape[2] - self.filter_size[1]) // self.stride[1] + 1,
            inputs.shape[3]
        )
        self.max_w = output_shape[1] * self.stride[0]
        self.max_h = output_shape[2] * self.stride[1]

        self.outputs = np.full(output_shape, -np.inf)

        for i in range(self.filter_size[0]):
            for j in range(self.filter_size[1]):
                input_slice = inputs[:, i:self.max_w+i:self.stride[0], j:self.max_h+j:self.stride[1], :]
                self.outputs = np.maximum(self.outputs, input_slice)

        return self.outputs


    def backward(self, output_grads, learning_rate):
        """
        Backward pass
        """        
        input_grads = np.zeros(self.inputs.shape)

        for i in range(self.filter_size[0]):
            for j in range(self.filter_size[1]):
                input_slice = self.inputs[:, i:self.max_w+i:self.stride[0], j:self.max_h+j:self.stride[1], :]
                mask = (input_slice == self.outputs)
                input_grads[:, i:self.max_w+i:self.stride[0], j:self.max_h+j:self.stride[1], :] += mask * output_grads

        self.inputs = None
        self.outputs = None
        
        return input_grads

###############################################################################
########################## ReLUActivationLayer.py #############################
###############################################################################

class ReLUActivationLayer:
    """
    ReLU activation layer
    """
    def __init__(self):
        pass

    def forward(self, inputs):
        """
        Forward pass
        """
        self.inputs = inputs
        outputs = np.maximum(0, inputs)
        return outputs


    def backward(self, output_grads, learning_rate):
        """
        Backward pass
        """
        assert self.inputs.shape == output_grads.shape

        input_grads = (self.inputs > 0) * output_grads
        self.inputs = None
        return input_grads

###############################################################################
########################## FlatteningLayer.py #################################
###############################################################################

class FlatteningLayer:
    """
    A flattening layer is a layer that flattens the input.
    """
    def __init__(self):
        pass

    def forward(self, inputs):
        """
        Forward pass
        """
        self.input_shape = inputs.shape
        outputs = inputs.reshape(self.input_shape[0], -1)
        return outputs

    def backward(self, output_grads, learning_rate):
        """
        Backward pass
        """
        input_grads = output_grads.reshape(self.input_shape)
        self.input_shape = None
        return input_grads


###############################################################################
########################## SofMaxLayer.py #####################################
###############################################################################

class SoftMaxLayer:
    """
    Softmax activation layer, should be last layer
    """
    def __init__(self):
        pass


    def forward(self, inputs):
        """
        Forward pass
        """
        assert len(inputs.shape) == 2
        
        inputs -= np.max(inputs, axis=1, keepdims=True)

        exp_inputs = np.exp(inputs)
        sum = np.sum(exp_inputs, axis=1, keepdims=True)
        sum[sum == 0] = 1

        self.outputs = exp_inputs / sum
        self.outputs += 1e-6
        self.outputs /= np.sum(self.outputs, axis=1, keepdims=True)

        return self.outputs


    def backward_from_output(self, outputs, learning_rate):
        """
        Backward pass
        """
        assert self.outputs.shape == outputs.shape

        input_grads = self.outputs - outputs

        self.outputs = None
        return input_grads


###############################################################################
########################## DenseLayer.py ######################################
###############################################################################

class DenseLayer:
    """
    A dense layer is a layer of neurons in a neural network.
    """
    def __init__(self, out_node_count, in_node_count = None):
        self.in_node_count = None
        self.out_node_count = out_node_count
        self.weights = None
        self.biases = None

        if in_node_count is not None:
            self.in_node_count = in_node_count
            self.weights = np.random.randn(self.out_node_count, self.in_node_count) / np.sqrt(self.in_node_count)
            self.biases = np.random.randn(self.out_node_count)


    def forward(self, inputs):
        """
        Forward pass
        """
        assert(len(inputs.shape) == 2)

        if self.in_node_count is None:
            self.in_node_count = inputs.shape[1]
            self.weights = np.random.randn(self.out_node_count, self.in_node_count) / np.sqrt(self.in_node_count)
            self.biases = np.random.randn(self.out_node_count)

        self.inputs = inputs
        outputs = self.weights @ inputs.reshape(-1, self.in_node_count, 1)
        outputs = outputs.reshape(-1, self.out_node_count) + self.biases

        return outputs


    def backward(self, output_grads, learning_rate):
        """
        Backward pass
        """
        input_grads = self.weights.T @ output_grads.reshape(-1, self.out_node_count, 1)
        input_grads = input_grads.reshape(-1, self.in_node_count)

        weight_grads = output_grads.reshape(-1, self.out_node_count, 1) @ self.inputs.reshape(-1, 1, self.in_node_count)
        weight_grads = weight_grads.sum(axis=0)
        self.weights -= learning_rate * weight_grads / self.inputs.shape[0]

        bias_grads = output_grads.sum(axis=0)
        self.biases -= learning_rate * bias_grads / self.inputs.shape[0]
        
        self.inputs = None

        return input_grads

###############################################################################
########################## ConvolutionLayer.py ################################
###############################################################################


class ConvolutionLayer(object):
    """
    Convolution layer
    """
    def __init__(self, num_channels, filter_size=(1, 1), stride=(1, 1), pad=0):
        self.filter_size = filter_size
        self.num_channels = num_channels
        self.stride = stride
        self.pad = pad

        assert pad < filter_size[0] and pad < filter_size[1]

        self.filters = None 
        self.biases = None


    def forward(self, inputs):
        """
        Forward pass
        """
        assert len(inputs.shape) == 4

        kh, kw = self.filter_size
        c = self.num_channels
        b, h, w, d = inputs.shape
        sh, sw = self.stride
        p = self.pad

        if self.filters is None:
            self.filters = np.random.randn(kh, kw, d, c) / np.sqrt(d)
            self.biases = np.random.randn(c)

        self.inputs = inputs
        inputs = inputs.reshape(b, h, w, 1, d)
        oh, ow = ((h + 2 * p - kh) // sh + 1, (w + 2 * p - kw) // sw + 1)
        outputs = np.full((b, oh, ow, c), self.biases)

        max_h = oh * sh
        max_w = ow * sw

        ### CONVOLUTION
        for i in range(0,kh):
            for j in range(0, kw):
                start_i = -self.pad + i
                end_i = start_i + max_h
                actual_start_i = start_i % sh if start_i < 0 else start_i
                actual_end_i = end_i - sh * max((end_i - h) // sh, 0)

                start_j = -self.pad + j
                end_j = start_j + max_w
                actual_start_j = start_j % sw if start_j < 0 else start_j
                actual_end_j = end_j - sw * max((end_j - w) // sw, 0)

                input_slice = inputs[:, actual_start_i:actual_end_i:sh, actual_start_j:actual_end_j:sw, :, :]
                kernel = self.filters[i, j, :, :]

                x =  input_slice @ kernel

                effected_outputs = outputs[:, (actual_start_i-start_i) // sh: (actual_end_i-start_i) // sh, (actual_start_j-start_j) // sw: (actual_end_j-start_j) // sw, :]
                effected_outputs += x.reshape(effected_outputs.shape)

        return outputs


    def backward(self, output_grads, learning_rate):
        """
        Backward pass
        """
        kh, kw = self.filter_size
        b, h, w, c = output_grads.shape
        assert c == self.num_channels
        sh, sw = self.stride
        assert b == self.inputs.shape[0]
        ih, iw, d = self.inputs.shape[1:]

        p = self.pad
        ph = kh - 1 - p
        pw = kw - 1 - p

        output_grads = output_grads.reshape(b, h, w, c, 1)
        oh, ow = (ih + kh - 1, iw + kw - 1) # shape of unpadded-padded dilated output_grads
        input_grads = np.zeros((b, ih, iw, d))

        ### CONVOLUTION for input_grads
        for i in range(0,kh):
            for j in range(0, kw):
                # NOTE: start, ends are from unpadded-padded dilated output_grads
                # actual are from dilated output_grads
                # undilated are from output_grads

                start_i = -ph + i
                end_i = start_i + ih
                actual_start_i = max(0, start_i)
                actual_end_i = min(ih+2*p-kh+1, end_i)
                undilated_start_i = (actual_start_i + sh - 1) // sh
                undilated_end_i = (actual_end_i + sh - 1) // sh

                start_j = -pw + j
                end_j = start_j + iw
                actual_start_j = max(0, start_j)
                actual_end_j = min(iw+2*p-kw+1, end_j)
                undilated_start_j = (actual_start_j + sw - 1) // sw
                undilated_end_j = (actual_end_j + sw - 1) // sw

                kernel = self.filters[kh - i - 1, kw - j - 1, :, :]
                output_grads_slice = output_grads[:, undilated_start_i:undilated_end_i, undilated_start_j:undilated_end_j, :, :]
                x = kernel @ output_grads_slice

                effected_start_i = (actual_start_i-start_i) + (-actual_start_i) % sh
                effected_end_i = effected_start_i + (undilated_end_i - undilated_start_i) * sh
                effected_start_j = (actual_start_j-start_j) + (-actual_start_j) % sw
                effected_end_j = effected_start_j + (undilated_end_j - undilated_start_j) * sw

                effected_input_grads = input_grads[:, effected_start_i : effected_end_i : sh, effected_start_j : effected_end_j : sw, :]
                effected_input_grads += x.reshape(effected_input_grads.shape)

 
        self.biases -= output_grads.sum(axis = (0, 1, 2)).reshape((c,)) * learning_rate / b

        ### CONVOLUTION for filters
        for i in range(kh):
            for j in range(kw):
                # NOTE: start, ends are from padded input
                # actual are from unpadded input
                # sparse are from unpadded input with stride (sh, sw)
                # o is from output_grads

                start_i = -p + i
                end_i = start_i + ih + 2*p - kh + 1
                actual_start_i = max(0, start_i)
                actual_end_i = min(ih, end_i)

                start_j = -p + j
                end_j = start_j + iw + 2*p - kw + 1
                actual_start_j = max(0, start_j)
                actual_end_j = min(iw, end_j)

                sparse_start_i = start_i + ((actual_start_i - start_i + sh-1) // sh) * sh 
                sparse_start_j = start_j + ((actual_start_j - start_j + sw-1) // sw) * sw

                inputs_slice = self.inputs.reshape(b, ih, iw, d, 1)[:, sparse_start_i:actual_end_i:sh, sparse_start_j:actual_end_j:sw, :]

                o_start_i = (actual_start_i - start_i + sh - 1) // sh
                o_end_i = (actual_end_i - start_i + sh - 1) // sh
                o_start_j = (actual_start_j - start_j + sw - 1) // sw
                o_end_j = (actual_end_j - start_j + sw - 1) // sw
            
                output_grads_slice = output_grads.reshape(b, h, w, 1, c)[:, o_start_i:o_end_i, o_start_j:o_end_j, :, :]
                self.filters[i, j, :, :] -= (inputs_slice * output_grads_slice).sum(axis = (0, 1, 2)) * learning_rate / b

        self.inputs = None
        return input_grads


###############################################################################
########################## CNN.py #############################################
###############################################################################


class CNN:
    """
    A simple Convolutional Neural Network
    """
    def __init__(self, epochs = 1, learning_rate = 0.1, batch_size = 1):
        """
        epochs: number of epochs to train
        learning_rate: learning rate
        batch_size: batch size
        """
        self.layers = []
        self.epochs = epochs
        self.learning_rate = learning_rate
        self.batch_size = batch_size 


    def addLayer(self, layer):
        """
        Add a layer to the network
        """
        self.layers.append(layer)


    def printShapes(self, X):
        """
        Print shapes of data after each layer when X is fed
        """
        print(X.shape[1:])
        for layer in self.layers:
            X = layer.forward(X)
            print(X.shape[1:])


    def report(self, X, y, Xv, yv, record, verbose, train_scores, validation_scores):
        """
        report scores
        """
        if record or verbose:
            predictions = self.predict_proba(X)

            loss = log_loss(y, predictions)
            acc = accuracy_score(np.argmax(y, axis=1), np.argmax(predictions, axis=1))
            macrof1 = f1_score(np.argmax(y, axis=1), np.argmax(predictions, axis=1), average='macro')

            if verbose:
                print("training Loss: {}".format(loss))
                print("training Accuracy: {}".format(acc))
                print("training Macro F1: {}".format(macrof1))

            if record:
                train_scores.append([loss, acc, macrof1])

            if Xv is not None and yv is not None:
                predictions = self.predict_proba(Xv)

                loss = log_loss(yv, predictions)
                acc = accuracy_score(np.argmax(yv, axis=1), np.argmax(predictions, axis=1))
                macrof1 = f1_score(np.argmax(yv, axis=1), np.argmax(predictions, axis=1), average='macro')

                if verbose:
                    print("validation Loss: {}".format(loss))
                    print("validation Accuracy: {}".format(acc))
                    print("validation Macro F1: {}".format(macrof1))

                if record:
                    validation_scores.append([loss, acc, macrof1])

            if verbose:
                print()
        return


    def train(self, X, y, Xv = None, yv = None, record = False, verbose = False):
        """
        Train the network
        """
        self.output_shape = y.shape[1]
        train_scores = []
        validation_scores = []

        self.report(X, y, Xv, yv, record, verbose, train_scores, validation_scores)

        for epoch in range(self.epochs):
            print("Epoch: {}/{}".format(epoch+1, self.epochs))
            bar = ProgressBar(60, 'Training')

            permutation = np.random.permutation(len(X))
            X = X[permutation]
            y = y[permutation]

            for i in range(0, len(X), self.batch_size):
                sz = min(self.batch_size, len(X) - i)
                X_batch = X[i:i+sz]
                y_batch = y[i:i+sz]

                output = X_batch
                for layer in self.layers:
                    output = layer.forward(output)

                last = True
                for layer in reversed(self.layers):
                    if last:
                        grads = layer.backward_from_output(y_batch, self.learning_rate)
                        last = False
                    else:
                        grads = layer.backward(grads, self.learning_rate)
                bar.update((i+sz) / len(X))

            self.report(X, y, Xv, yv, record, verbose, train_scores, validation_scores)
        return train_scores, validation_scores


    def predict_proba(self, X):
        """
        Predict probabilities for each class
        """
        probs = np.zeros((len(X), self.output_shape))

        bar = ProgressBar(60, 'Predicting')
        for i in range(0, len(X), self.batch_size):
            sz = min(self.batch_size, len(X) - i)
            output = X[i:i+sz]
            for layer in self.layers:
                output = layer.forward(output)
            probs[i:i+sz] = output

            bar.update((i+sz) / len(X))

        return probs


    def predict(self, X):
        """
        Predict class labels
        """
        return np.argmax(self.predict_proba(X), axis=1)

    


###############################################################################
########################## progress_bar.py ####################################
###############################################################################

class ProgressBar:
    """
    A simple progress bar
    """
    def __init__(self, bar_count, title = None):
        """
        bar_count: number of bars in progress bar
        """
        self.bar_count = bar_count
        self.cur_progress = 0
        if title is not None:
            print(title + ': ', end='', flush=True)


    def update(self, progress):
        """
        update progress bar with progress ratio in (0, 1)
        """
        progress = round(progress * self.bar_count)
        while progress > self.cur_progress and self.cur_progress < self.bar_count:
            print('|', end='', flush=True)
            self.cur_progress += 1
            if self.cur_progress == self.bar_count:
                print('')

###############################################################################
########################## data_handler.py ####################################
###############################################################################

def process_image(image, dilation_shape, image_shape):
    """
    process single image for model
    """
    image = 255 - image
    image -= np.min(image)

    m = cv2.moments(image, False)
    cx, cy = int(m['m10']/m['m00']), int(m['m01']/m['m00'])

    left, right = cx, image.shape[1] - cx
    up, down = cy, image.shape[0] - cy
    min_dist = min(left, right, up, down)
    image = image[cy - min_dist:cy + min_dist, cx - min_dist:cx + min_dist]

    kernel = np.ones(dilation_shape, np.uint8)
    image = cv2.dilate(image, kernel, iterations=1)
    
    image = image / 255.0
    image = cv2.resize(image, image_shape)
    image /= np.max(image)
    return image


def read_csv_data(csv_path):
    """
    reads csv data from single file
    """
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

    return mapping

def read_images(image_folder, image_shape, dilation_shape, mapping = None):
    """
    read images from folder with or without mapping
    """
    print("Reading images from folder: " + image_folder)
    names = []
    images = []
    labels = [] if mapping is not None else None

    total_file_count = len(os.listdir(image_folder))
    progress_bar = ProgressBar(60, "Reading images")

    for file_name in os.listdir(image_folder):
        file_path = os.path.join(image_folder, file_name)
        if file_name.endswith(".csv"):
            continue
        if mapping is not None and file_name not in mapping:
            continue

        names.append(file_name)

        image = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)
        image = process_image(image, dilation_shape=dilation_shape, image_shape=image_shape)
        images.append(image.reshape(image_shape + (1,)))

        if mapping is not None:
            digit = mapping[file_name]
            encoding = np.zeros(10)
            encoding[digit] = 1
            labels.append(encoding)

        progress_bar.update(len(images) / total_file_count)

    return names, images, labels


def load_image_data(image_folder, csv_path, image_shape, dilation_shape):
    """
    loads image data from single folder
    """
    mapping = read_csv_data(csv_path) if csv_path is not None else None
    names, images, labels = read_images(image_folder, image_shape, dilation_shape, mapping)

    images_cnt = len(images)
    print("Loaded {} images".format(images_cnt))
    print()

    images = np.array(images)
    labels = np.array(labels) if labels is not None else None

    return names, images, labels



###############################################################################
########################## 1705076_test.py ####################################
###############################################################################

if __name__ == "__main__":
    assert len(sys.argv) >= 2

    show_probs = len(sys.argv) >= 3

    path_to_folder = sys.argv[1]
    names, images, _ = load_image_data(path_to_folder, csv_path = None, image_shape=(28, 28), dilation_shape=(2, 2))

    cnn = pickle.load(open("1705076_model.pickle", "rb"))

    predictions = cnn.predict(images)

    if show_probs:
        probs = cnn.predict_proba(images)
        for i in range(len(names)):
            plt.imshow(images[i], cmap='gray')
            plt.show()
            print(names[i], predictions[i], ' - ', ["{0:.3f}".format(x) for x in probs[i]])

    csv_file = os.path.join(path_to_folder, "1705076_predictions.csv")

    with open(csv_file, 'w', newline='') as f:
        writer = csv.writer(f)

        writer.writerow(["FileName", "Digit"])

        for i in range(len(names)):
            writer.writerow([names[i], predictions[i]])

    print("predictions saved")