import numpy as np

class DenseLayer:
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
        input_grads = self.weights.T @ output_grads.reshape(-1, self.out_node_count, 1)
        input_grads = input_grads.reshape(-1, self.in_node_count)

        weight_grads = output_grads.reshape(-1, self.out_node_count, 1) @ self.inputs.reshape(-1, 1, self.in_node_count)
        weight_grads = weight_grads.sum(axis=0)
        self.weights -= learning_rate * weight_grads / self.inputs.shape[0]

        bias_grads = output_grads.sum(axis=0)
        self.biases -= learning_rate * bias_grads / self.inputs.shape[0]
        
        self.inputs = None

        return input_grads