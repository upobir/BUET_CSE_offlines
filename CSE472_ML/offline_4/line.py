from CNN import *
from ConvolutionLayer import *
from ReLUActivationLayer import *
from DenseLayer import *
from SoftMaxLayer import *
from FlatteningLayer import *
from MaxPoolingLayer import *
import pickle
import os

def get_bars(h, w):
    X = np.random.normal(0, 0.2, (h, w, 2))
    p = np.random.randint(h-1)
    q = np.random.randint(w-1)
    X[p:p+2, q, 0] = np.random.normal(1, 0.2, (2,))
    X[p, q:q+2, 1] = np.random.normal(1, 0.2, (2,))
    return X

def get_square(h, w):
    X = np.random.normal(0, 0.2, (h, w, 2))
    p = np.random.randint(h-1)
    q = np.random.randint(w-1)
    X[p:p+2, q:q+2, 0] = np.random.normal(1, 0.2, (2, 2))
    X[p:p+2, q:q+2, 1] = np.random.normal(1, 0.2, (2, 2))
    return X

def get_random(h, w):
    X = np.random.normal(0.5, 0.25, (h, w, 2))
    return X

N = 10000

X = np.zeros((N, 5, 4, 2))

y = np.zeros((N, 3))

for i in range(N):
    num = np.random.randint(3)

    if num == 0:
        X[i] = get_bars(5, 4)
        y[i] = [1, 0, 0]
    elif num == 1:
        X[i] = get_square(5, 4)
        y[i] = [0, 1, 0]
    else:
        X[i] = get_random(5, 4)
        y[i] = [0, 0, 1]

cnn = None

if os.path.isfile('line.pkl'):
    filename = 'line.pkl'
    cnn = pickle.load(open(filename, 'rb'))
else:
    cnn = CNN(epochs=200, learning_rate=0.0005, batch_size=32)
    cnn.addLayer(ConvolutionLayer(8, (2, 2), (1, 1), 0))
    cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
    cnn.addLayer(ReLUActivationLayer())
    cnn.addLayer(ConvolutionLayer(3, (2, 2), (1, 1), 0))
    cnn.addLayer(ReLUActivationLayer())
    cnn.addLayer(FlatteningLayer())
    cnn.addLayer(DenseLayer(5))
    cnn.addLayer(ReLUActivationLayer())
    cnn.addLayer(DenseLayer(8))
    cnn.addLayer(ReLUActivationLayer())
    cnn.addLayer(DenseLayer(3))
    cnn.addLayer(SoftMaxLayer())

    cnn.train(X, y)

test = np.array([
    get_bars(5, 4),
    get_square(5, 4),
    get_random(5, 4),
])

res = cnn.predict(test)
print(res)

# save the model to disk as "line.pkl"
filename = 'line.pkl'
pickle.dump(cnn, open(filename, 'wb'))