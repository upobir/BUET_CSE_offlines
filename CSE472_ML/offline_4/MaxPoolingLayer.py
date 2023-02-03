import numpy as np

class MaxPoolingLayer:
    def __init__(self, filter_size = (2, 2), stride = (1, 1)):
        self.filter_size = filter_size
        self.stride = stride

    def forward(self, inputs):
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
        
        input_grads = np.zeros(self.inputs.shape)

        for i in range(self.filter_size[0]):
            for j in range(self.filter_size[1]):
                input_slice = self.inputs[:, i:self.max_w+i:self.stride[0], j:self.max_h+j:self.stride[1], :]
                mask = (input_slice == self.outputs)
                input_grads[:, i:self.max_w+i:self.stride[0], j:self.max_h+j:self.stride[1], :] += mask * output_grads

        return input_grads