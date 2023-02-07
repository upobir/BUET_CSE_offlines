import numpy as np

class FlatteningLayer:
    def __init__(self):
        pass

    def forward(self, inputs):
        self.input_shape = inputs.shape
        outputs = inputs.reshape(self.input_shape[0], -1)
        return outputs

    def backward(self, output_grads, learning_rate):
        input_grads = output_grads.reshape(self.input_shape)
        self.input_shape = None
        return input_grads