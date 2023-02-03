import sys
import os
 
current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
sys.path.append(parent)

from ConvolutionLayer import ConvolutionLayer
import unittest
import traceback
import numpy as np

class TestConvolutionLayer(unittest.TestCase):
    def setUp(self):
        self.layer = ConvolutionLayer(3, filter_size=(3, 2), stride=(1, 2), pad=1)
        self.layer.filters = np.array([
            [[[1, 2, 1], [1, 0, -1]], [[1, 2, 0], [1, 0, 0]]],
            [[[1, 0, 2], [1, 1, 0]], [[1, 0, 0], [1, 1, 0]]],
            [[[1, 2, 0], [1, 0, 0]], [[1, 2, -1], [1, 0, 1]]],
        ], dtype=np.float32)
        
        self.layer.biases = np.array([
            [1, 0, -1],
        ], dtype=np.float32)

        self.inputs = np.array([
            [
                [[-1, 2], [1, -3], [2, -4], [3, 3], [-3, 2]],
                [[8, -1], [2, 4], [-5, 6], [-2, -3], [1, 0]],
                [[1, 0], [2, -4], [5, -6], [2, 2], [-2, 0]],
                [[1, 2], [3, -3], [3, -2], [7, -3], [3, 4]],
            ],
            [
                [[1, 2], [6, -3], [2, -4], [1, 6], [3, 2]],
                [[3, -1], [2, 4], [0, 3], [-2, -3], [0, 2]],
                [[2, 0], [4, -4], [5, -2], [2, 3], [-4, 0]],
                [[3, 2], [0, -3], [3, -2], [8, -3], [1, 6]],
            ]
        ])

        self.outputs = np.array([
            [
                [[8,18,-9], [3,-13,13], [1,3,5]],
                [[9,-1,-1], [0,30,-3], [3,-3,-2]],
                [[11,18,1], [5,-4,-3], [9,20,6]],
                [[4,4,0], [-2,9,12], [13,1,14]],
            ],
            [
                [[5,8,-4], [10,-3,15], [9,4,4]],
                [[7,5,-2], [13,41,6], [10,3,-5]],
                [[9,12,-1], [10,4,1], [10,17,10]],
                [[7,6,0], [1,13,8], [13,-1,15]],
            ]
        ]) + self.layer.biases

        self.output_grads = np.array([
            [
                [[1,0,1], [0,1,0], [1,0,1]],
                [[0,1,0], [-1,0,1], [0,1,0]],
                [[1,0,1], [0,-1,0], [1,0,-1]],
                [[0,1,0], [1,0,1], [0,1,0]],
            ],
            [
                [[1,0,1], [0,-1,0], [1,0,1]],
                [[0,-1,0], [1,0,1], [0,1,0]],
                [[1,0,-1], [0,1,0], [1,0,1]],
                [[0,1,0], [1,0,-1], [0,-1,0]],
            ]
        ])
        self.input_grads = np.array([
            [
                [[3, 1], [0, -1], [-1, 0], [5, 1], [3, 1]],
                [[1, 4], [1, -1], [-1, -1], [1, 4], [1, 4]],
                [[5, 1], [1, -2], [-1, 0], [3, 1], [5, 1]],
                [[0, 3], [1, 1], [-1, 1], [1, 2], [2, 1]],
            ],
            [
                [[-1, 1], [2, -1], [1, 0], [5, 1], [3, 1]],
                [[1, 2], [3, 1], [1, 1], [3, 2], [1, 4]],
                [[1, 1], [1, 4], [1, 4], [3, 1], [1, 1]],
                [[2, 1], [1, 1], [3, 1], [1, 0], [0, 1]],
            ]
        ], dtype=np.float32)

        self.new_biases = self.layer.biases - 0.1 * np.array([
            [10, 4, 6]
        ], dtype=np.float32) / 2.0

        # TODO
        self.new_filters = self.layer.filters - 0.1 * np.array([
            [[[7, 4, 5], [-14, 8, -6]], [[22, 8, 8], [-8, 1, -10]]],
            [[[11, -8, 11], [8, -6, 18]], [[8, 12, -8], [1, 8, 17]]],
            [[[13, 1, 3], [-12, 5, -14]], [[20, -12, 18], [18, 3, -6]]],
        ], dtype=np.float32) / 2.0

    def test_shape_1(self):
        self.layer = ConvolutionLayer(3, filter_size=(3, 2), stride=(1, 2), pad=1)
        
        try:
            self.layer.forward(self.inputs)
            self.assertEqual(self.layer.filters.shape, (3, 2, 2, 3))
            self.assertEqual(self.layer.biases.shape, (3,))

            self.layer.backward(self.output_grads, 0.1)
        except Exception as e:
            print(traceback.format_exc())
            self.fail(e)

    def test_shape_2(self):

        self.layer = ConvolutionLayer(2, filter_size=(3, 2), stride=(2, 2), pad=1)

        try:
            inputs = np.random.randn(5, 6, 4, 3)
            output = self.layer.forward(inputs)
            self.assertEqual(self.layer.filters.shape, (3, 2, 3, 2))
            self.assertEqual(self.layer.biases.shape, (2,))

            output_grads = np.random.randn(*output.shape)
            self.layer.backward(output_grads, 0.1)
        except Exception as e:
            print(traceback.format_exc())
            self.fail(e)

    def test_forward(self):

        actual = self.layer.forward(self.inputs)
        expected = self.outputs


        self.assertTrue(np.allclose(actual, expected))

    def test_backward(self):

        old_weights = self.layer.filters.copy()

        self.layer.forward(self.inputs)

        actual = self.layer.backward(self.output_grads, 0.1)
        expected = self.input_grads

        self.assertTrue(np.allclose(actual, expected)) 
        self.assertTrue(np.allclose(self.layer.biases, self.new_biases))
        self.assertTrue(np.allclose(self.layer.filters, self.new_filters))
