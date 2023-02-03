import sys
import os
 
current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
sys.path.append(parent)

from DenseLayer import DenseLayer
import unittest
import numpy as np

class TestDenseLayer(unittest.TestCase):

    def setUp(self):
        self.layer = DenseLayer(3, in_node_count=2)
        self.layer.weights = np.array([
            [1, -2],
            [0, 3],
            [-3, 2]
        ], dtype=np.float64)
        self.layer.biases = np.array([
            -1,
            3,
            0,
        ], dtype=np.float64)

        self.inputs = np.array([ 
            [1, 2],
            [3, 4],
            [5, 6],
        ])
        self.outputs = np.array([
            [-4, 9, 1], 
            [-6, 15, -1],
            [-8, 21, -3],
        ])
        self.output_grads = np.array([
            [1, 0, -1],
            [2, -3, 3],
            [-1, 2, -4],
        ])
        self.input_grads = np.array([ 
            [4, -4],
            [-7, -7],
            [11, 0],
        ])

        self.new_weights = self.layer.weights - 0.1 * np.array([
            [2, 4],
            [1, 0],
            [-12, -14],
        ]) / 3.0
        self.new_biases = self.layer.biases - 0.1 * np.array([
            2, 
            -1,
            -2,
        ]) / 3.0

    def test_shapes(self):
        self.layer = DenseLayer(3)
        
        try:
            self.layer.forward(self.inputs)
            self.assertEqual(self.layer.weights.shape, (3, 2))
            self.assertEqual(self.layer.biases.shape, (3,))
        except Exception as e:
            self.fail(e)

    def test_forward(self):

        actual = self.layer.forward(self.inputs)
        expected = self.outputs

        self.assertTrue(np.allclose(actual, expected))


    def test_backward(self):
        self.layer.forward(self.inputs)

        actual = self.layer.backward(self.output_grads, 0.1)
        expected = self.input_grads

        self.assertTrue(np.allclose(actual, expected))
        self.assertTrue(np.allclose(self.layer.weights, self.new_weights))
        self.assertTrue(np.allclose(self.layer.biases, self.new_biases))