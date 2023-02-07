import numpy as np

class SoftMaxLayer:
    def __init__(self):
        pass

    def forward(self, inputs):
        assert len(inputs.shape) == 2
        
        # subtract max from each row
        inputs -= np.max(inputs, axis=1, keepdims=True)

        exp_inputs = np.exp(inputs)
        sum = np.sum(exp_inputs, axis=1, keepdims=True)
        sum[sum == 0] = 1

        self.outputs = exp_inputs / sum

        self.outputs += 1e-6
        self.outputs /= np.sum(self.outputs, axis=1, keepdims=True)

        return self.outputs


    def backward_from_output(self, outputs, learning_rate):
        assert self.outputs.shape == outputs.shape


        input_grads = self.outputs - outputs

        self.outputs = None
        
        return input_grads