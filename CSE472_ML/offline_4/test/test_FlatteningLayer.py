import sys
import os
 
current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
sys.path.append(parent)

from FlatteningLayer import FlatteningLayer
import unittest
import numpy as np

class TestFlatteningLayer(unittest.TestCase):

    def setUp(self):
        self.layer = FlatteningLayer()

        self.inputs = np.array([ 
            [ [[1, 2], [3, 4]], [[5, 6],[7, 8]] ],
            [ [[10, 20], [30, 40]], [[50, 60],[70, 80]] ] 
        ])
        self.outputs = np.array([
            [1, 2, 3, 4, 5, 6, 7, 8], 
            [10, 20, 30, 40, 50, 60, 70, 80],
        ])
        self.output_grads = np.array([
            [1, 0, 2, 0, -1, 2, 3, 1],
            [4, 1, -3, 4, 7, 0, -3, 1],
        ])
        self.input_grads = np.array([
            [ [[1, 0], [2, 0]], [[-1, 2], [3, 1]] ],
            [ [[4, 1], [-3, 4]], [[7, 0], [-3, 1]] ],
        ])


    def test_forward(self):

        actual = self.layer.forward(self.inputs)
        expected = self.outputs

        self.assertTrue(np.allclose(actual, expected))


    def test_backward(self):
        self.layer.forward(self.inputs)

        actual = self.layer.backward(self.output_grads, 0.1)
        expected = np.array([
            [ [[1, 0], [2, 0]], [[-1, 2], [3, 1]] ],
            [ [[4, 1], [-3, 4]], [[7, 0], [-3, 1]] ],
        ])

        self.assertTrue(np.allclose(actual, expected))