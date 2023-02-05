import numpy as np
from progress_bar import ProgressBar
from metrics import cross_entropy

class CNN:
    def __init__(self, epochs = 1, learning_rate = 0.1, batch_size = 1):
        self.layers = []
        self.epochs = epochs
        self.learning_rate = learning_rate
        self.batch_size = batch_size

    def addLayer(self, layer):
        self.layers.append(layer)

    def train(self, X, y):
        for epoch in range(self.epochs):
            print("Epoch: {}/{}".format(epoch+1, self.epochs))
            bar = ProgressBar(70)

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

            loss = cross_entropy(y, self.predict_proba(X))
            print("Loss: {}".format(loss))

        

    def predict_proba(self, X):
        output = X
        for layer in self.layers:
            output = layer.forward(output)
        return output

    def predict(self, X):
        return np.argmax(self.predict_proba(X), axis=1)

    