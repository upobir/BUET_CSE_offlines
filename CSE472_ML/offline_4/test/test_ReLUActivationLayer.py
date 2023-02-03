import sys
import os
 
current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
sys.path.append(parent)

from ReLUActivationLayer import ReLUActivationLayer
import unittest
import numpy as np

class TestReLUActivationLayer(unittest.TestCase):

    def setUp(self):
        self.layer = ReLUActivationLayer()

        self.inputs = np.array([[1, -2, 3], [4, 5, -6], [0, 7, -1]])
        self.outputs = np.array([[1, 0, 3], [4, 5, 0], [0, 7, 0]])
        self.output_grads = np.array([[1, -1, 2], [3, 0, -2], [0, -1, 3]])
        self.input_grads = np.array([[1, 0, 2], [3, 0, 0], [0, -1, 0]])

    def test_forward(self):

        actual = self.layer.forward(self.inputs)
        expected = self.outputs

        self.assertTrue(np.allclose(actual, expected))

    def test_backward(self):

        self.layer.forward(self.inputs)

        actual = self.layer.backward(self.output_grads, 0.1)
        expected = self.input_grads

        self.assertTrue(np.allclose(actual, expected))
