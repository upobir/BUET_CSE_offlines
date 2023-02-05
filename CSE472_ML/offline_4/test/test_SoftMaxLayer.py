import sys
import os
import math
 
current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
sys.path.append(parent)

from SoftMaxLayer import SoftMaxLayer
import unittest
import numpy as np

class TestSoftMaxLayer(unittest.TestCase):

    def setUp(self):
        self.layer = SoftMaxLayer()
        self.inputs = np.array([
            [2, 1, 0, -2],
            [0.5, 3, -1, -1],
        ])

        s1 = math.exp(2) + math.exp(1) + math.exp(0) + math.exp(-2)
        s2 = math.exp(0.5) + math.exp(3) + math.exp(-1) + math.exp(-1)

        self.outputs = np.array([
            [math.exp(2)/s1, math.exp(1)/s1, math.exp(0)/s1, math.exp(-2)/s1],
            [math.exp(0.5)/s2, math.exp(3)/s2, math.exp(-1)/s2, math.exp(-1)/s2],
        ])
        self.outputs += 1e-6
        self.outputs /= np.sum(self.outputs, axis=1, keepdims=True)

        self.actual_outputs = np.array([
            [1, 0, 0, 0],
            [0, 0, 1, 0],
        ])
        self.input_grads = self.outputs - self.actual_outputs


    def test_forward(self):
        actual = self.layer.forward(self.inputs)
        expected = self.outputs

        self.assertTrue(np.allclose(actual, expected))


    def test_backward_from_output(self):
        self.layer.forward(self.inputs)

        expected = self.input_grads
        actual = self.layer.backward_from_output(self.actual_outputs, 0.1)

        self.assertTrue(np.allclose(actual, expected))
