import numpy as np
from progress_bar import ProgressBar
# from metrics import cross_entropy, accuracy
from sklearn.metrics import accuracy_score, log_loss, f1_score

class CNN:
    def __init__(self, epochs = 1, learning_rate = 0.1, batch_size = 1):
        self.layers = []
        self.epochs = epochs
        self.learning_rate = learning_rate
        self.batch_size = batch_size 

    def addLayer(self, layer):
        self.layers.append(layer)

    def printShapes(self, X):
        print(X.shape[1:])
        for layer in self.layers:
            X = layer.forward(X)
            print(X.shape[1:])

    def train(self, X, y, Xv = None, yv = None, record = False):
        self.output_shape = y.shape[1]
        records = []
        for epoch in range(self.epochs):
            print("Epoch: {}/{}".format(epoch+1, self.epochs))
            bar = ProgressBar(75)

            permutation = np.random.permutation(len(X))
            X = X[permutation]
            y = y[permutation]

            for i in range(0, len(X), self.batch_size):
                sz = min(self.batch_size, len(X) - i)
                X_batch = X[i:i+sz]
                y_batch = y[i:i+sz]

                # print('FORWARD' +  '-' * 43)
                output = X_batch
                cnt = 0
                for layer in self.layers:
                    cnt += 1
                    # print('layer' + str(cnt))
                    output = layer.forward(output)

                # print('BACK' +  '-' * 44)

                last = True
                cnt = 0
                for layer in reversed(self.layers):
                    cnt += 1
                    # print('layer' + str(cnt))
                    if last:
                        grads = layer.backward_from_output(y_batch, self.learning_rate)
                        last = False
                    else:
                        grads = layer.backward(grads, self.learning_rate)

                bar.update((i+sz) / len(X))


            if record or epoch % 2 == 0:
                predictions = self.predict_proba(X, self.batch_size)
                loss = log_loss(y, predictions)
                print("training Loss: {}".format(loss))
                acc = accuracy_score(np.argmax(y, axis=1), np.argmax(predictions, axis=1))
                print("training Accuracy: {}".format(acc))
                macrof1 = f1_score(np.argmax(y, axis=1), np.argmax(predictions, axis=1), average='macro')
                print("training Macro F1: {}".format(macrof1))

                records.append([loss, acc, macrof1])

                if Xv is not None and yv is not None:
                    predictions = self.predict_proba(Xv, self.batch_size)
                    loss = log_loss(yv, predictions)
                    print("validation Loss: {}".format(loss))
                    acc = accuracy_score(np.argmax(yv, axis=1), np.argmax(predictions, axis=1))
                    print("validation Accuracy: {}".format(acc))
                    macrof1 = f1_score(np.argmax(yv, axis=1), np.argmax(predictions, axis=1), average='macro')
                    print("validation Macro F1: {}".format(macrof1))

                    records[-1] += [loss, acc, macrof1]

            print()

        return records

    def predict_proba(self, X, batch_size = None):
        batch_size = len(X) if batch_size is None else batch_size
        probs = np.zeros((len(X), self.output_shape))

        for i in range(0, len(X), batch_size):
            sz = min(batch_size, len(X) - i)
            output = X[i:i+sz]
            for layer in self.layers:
                output = layer.forward(output)

            probs[i:i+sz] = output

        return probs

    def predict(self, X, batch_size = None):
        return np.argmax(self.predict_proba(X, batch_size), axis=1)

    