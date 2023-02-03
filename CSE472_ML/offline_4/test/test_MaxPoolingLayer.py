import sys
import os
 
current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
sys.path.append(parent)

from MaxPoolingLayer import MaxPoolingLayer
import unittest
import numpy as np

class TestMaxPoolingLayer(unittest.TestCase):

    def setUp(self):
        self.layer = MaxPoolingLayer(filter_size=(3, 2), stride=(1, 2))

        self.inputs = np.array([
            [
                [[-1, 2], [3, -4], [5, -6], [7, 8], [9, -10]],
                [[11, -12], [13, 14], [-15, 16], [17, -18], [-19, 20]],
                [[21, -22], [23, -24], [-25, 26], [27, 28], [-29, -30]],
                [[-31, 32], [33, -34], [35, 36], [37, -38], [39, -40]],
            ],
            [
                [[1, 2], [3, -4], [-5, 6], [-7, -8], [9, -10]],
                [[11, -12], [13, 14], [-15, 16], [-17, 18], [19, 20]],
                [[-21, -22], [23, -24], [-25, -26], [-27, -28], [29, -30]],
                [[-31, 32], [-33, 34], [35, -36], [-37, -38], [39, -40]],
            ]
        ])
        self.outputs = np.array([
            [
                [[23, 14], [27, 28]],
                [[33, 32], [37, 36]],
            ],
            [
                [[23, 14], [-5, 18]],
                [[23, 34], [35, 18]],
            ]
        ])
        self.output_grads = np.array([
            [
                [[1, -1], [2, 2]],
                [[-3, 3], [7, -3]],
            ],
            [
                [[7, 2], [-3, -4]],
                [[3, 4], [1, -2]],
            ]
        ])
        self.input_grads = np.array([
            [
                [[0, 0], [0, 0], [0, 0], [0, 0], [0, 0]],
                [[0, 0], [0, -1], [0, 0], [0, 0], [0, 0]],
                [[0, 0], [1, 0], [0, 0], [2, 2], [0, 0]],
                [[0, 3], [-3, 0], [0, -3], [7, 0], [0, 0]],
            ],
            [
                [[0, 0], [0, 0], [-3, 0], [0, 0], [0, 0]],
                [[0, 0], [0, 2], [0, 0], [0, -6], [0, 0]],
                [[0, 0], [10, 0], [0, 0], [0, 0], [0, 0]],
                [[0, 0], [0, 4], [1, 0], [0, 0], [0, 0]],
            ]
        ])

    def test_forward(self):

        actual = self.layer.forward(self.inputs)
        expected = self.outputs

        self.assertTrue(np.allclose(actual, expected))

    def test_backward(self):

        self.layer.forward(self.inputs)

        actual = self.layer.backward(self.output_grads, 0.1)
        expected = self.input_grads

        self.assertTrue(np.allclose(actual, expected))
