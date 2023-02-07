import numpy as np

class ReLUActivationLayer:
    def __init__(self):
        pass

    def forward(self, inputs):
        self.inputs = inputs
        outputs = np.maximum(0, inputs)
        return outputs

    def backward(self, output_grads, learning_rate):
        assert self.inputs.shape == output_grads.shape

        input_grads = (self.inputs > 0) * output_grads # np.where(self.inputs > 0, 1, 0)
        self.inputs = None
        return input_grads