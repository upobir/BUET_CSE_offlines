import numpy as np

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
            permutation = np.random.permutation(len(X))
            X = X[permutation]
            y = y[permutation]

            for i in range(0, len(X), self.batch_size):
                sz = min(self.batch_size, len(X) - i)
                X_batch = X[i:i+sz]
                y_batch = y[i:i+sz]

                # print('FORWARD' +  '-' * 43)
                output = X_batch
                for layer in self.layers:
                    output = layer.forward(output)

                # print('BACK' +  '-' * 44)

                last = True
                for layer in reversed(self.layers):
                    if last:
                        grads = layer.backward_from_output(y_batch, self.learning_rate)
                        last = False
                    else:
                        grads = layer.backward(grads, self.learning_rate)
                    
                # input()
                # print('-' * 50)
                # print('EPOCH' +  '-' * 45)
                # print('-' * 50)
        

    def predict(self, X):
        output = X
        for layer in self.layers:
            output = layer.forward(output)
        return output

    