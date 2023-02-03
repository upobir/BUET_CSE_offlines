import numpy as np

class SoftMaxLayer:
    def __init__(self):
        pass

    def forward(self, inputs):
        assert len(inputs.shape) == 2

        self.inputs = inputs
        exp_inputs = np.exp(inputs)
        exp_inputs[exp_inputs == 0] = 1

        self.outputs = exp_inputs / np.sum(exp_inputs, axis=1, keepdims=True)
        # print("output")
        # print(self.outputs)
        # print("-" * 50)
        return self.outputs


    def backward_from_output(self, outputs, learning_rate):
        assert self.outputs.shape == outputs.shape

        # print(self.outputs)
        # print(outputs)

        input_grads = self.outputs - outputs
        # print("input_grads")
        # print(input_grads)
        # print("-" * 50)
        return input_grads