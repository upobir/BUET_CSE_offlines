import numpy as np

class ConvolutionLayer(object):
    def __init__(self, num_channels, filter_size=(1, 1), stride=(1, 1), pad=0):
        self.filter_size = filter_size
        self.num_channels = num_channels
        self.stride = stride
        self.pad = pad

        assert pad < filter_size[0] and pad < filter_size[1]

        self.filters = None 
        self.biases = None

    def forward(self, inputs):
        assert len(inputs.shape) == 4

        kh, kw = self.filter_size
        c = self.num_channels
        b, h, w, d = inputs.shape
        sh, sw = self.stride
        p = self.pad

        if self.filters is None:
            self.filters = np.random.randn(kh, kw, d, c)
            self.biases = np.random.randn(c)

        self.inputs = inputs
        inputs = inputs.reshape(b, h, w, d, 1)
        oh, ow = ((h + 2 * p - kh) // sh + 1, (w + 2 * p - kw) // sw + 1)
        outputs = np.full((b, oh, ow, c), self.biases)

        max_h = oh * sh
        max_w = ow * sw

        for i in range(0,kh):
            for j in range(0, kw):
                start_i = -self.pad + i
                end_i = start_i + max_h
                actual_start_i = start_i % sh if start_i < 0 else start_i
                actual_end_i = end_i - sh * max((end_i - h) // sh, 0)

                start_j = -self.pad + j
                end_j = start_j + max_w
                actual_start_j = start_j % sw if start_j < 0 else start_j
                actual_end_j = end_j - sw * max((end_j - w) // sw, 0)

                input_slice = inputs[:, actual_start_i:actual_end_i:sh, actual_start_j:actual_end_j:sw, :, :]
                kernel = self.filters[i, j, :, :]

                x =  input_slice * kernel
                x = x.sum(axis = 3)

                effected_outputs = outputs[:, (actual_start_i-start_i) // sh: (actual_end_i-start_i) // sh, (actual_start_j-start_j) // sw: (actual_end_j-start_j) // sw, :]

                effected_outputs += x

        return outputs

    def backward(self, output_grads, learning_rate):
        kh, kw = self.filter_size
        b, h, w, c = output_grads.shape
        assert c == self.num_channels
        sh, sw = self.stride
        assert b == self.inputs.shape[0]
        ih, iw, d = self.inputs.shape[1:]

        # convolve the output_grads with the reversed filters with pad = (kh-1-p, kw-1-p), dilation = (sh-1, sw-1), stride = (1, 1)
        p = self.pad

        ph = kh - 1 - p
        pw = kw - 1 - p

        output_grads = output_grads.reshape(b, h, w, 1, c)
        oh, ow = (ih + kh - 1, iw + kw - 1) # shape of unpadded-padded dilated output_grads
        input_grads = np.zeros((b, ih, iw, d)) # (b, ih, iw, d)

        for i in range(0,kh):
            for j in range(0, kw):
                # NOTE: start, ends are from unpadded-padded dilated output_grads
                # actual are from dilated output_grads
                # undilated are from output_grads

                start_i = -ph + i
                end_i = start_i + ih
                actual_start_i = max(0, start_i)
                actual_end_i = min(ih+2*p-kh+1, end_i)
                undilated_start_i = (actual_start_i + sh - 1) // sh
                undilated_end_i = (actual_end_i + sh - 1) // sh

                start_j = -pw + j
                end_j = start_j + iw
                actual_start_j = max(0, start_j)
                actual_end_j = min(iw+2*p-kw+1, end_j)
                undilated_start_j = (actual_start_j + sw - 1) // sw
                undilated_end_j = (actual_end_j + sw - 1) // sw

                kernel = self.filters[kh - i - 1, kw - j - 1, :, :]
                output_grads_slice = output_grads[:, undilated_start_i:undilated_end_i, undilated_start_j:undilated_end_j, :, :]
                x = output_grads_slice * kernel
                x = x.sum(axis = 4)

                effected_start_i = (actual_start_i-start_i) + (-actual_start_i) % sh
                effected_end_i = effected_start_i + (undilated_end_i - undilated_start_i) * sh
                effected_start_j = (actual_start_j-start_j) + (-actual_start_j) % sw
                effected_end_j = effected_start_j + (undilated_end_j - undilated_start_j) * sw

                effected_input_grads = input_grads[:, effected_start_i : effected_end_i : sh, effected_start_j : effected_end_j : sw, :]
                effected_input_grads += x

 
        self.biases -= output_grads.sum(axis = (0, 1, 2)).reshape((c,)) * learning_rate / b


        for i in range(kh):
            for j in range(kw):
                # padded input size is (ih + 2 * p, iw + 2 * p)
                # convolving padded input with output_grads with dilation (sh-1, sw-1) and stride (1, 1) gives the gradient of the filter
                # dilated output_grads size is (ih + 2*p - kh + 1, iw + 2*p - kw + 1)

                # NOTE: start, ends are from padded input
                # actual are from unpadded input
                # sparse are from unpadded input with stride (sh, sw)
                # o is from output_grads

                # print(i, j, '-'*20)

                start_i = -p + i
                end_i = start_i + ih + 2*p - kh + 1
                actual_start_i = max(0, start_i)
                actual_end_i = min(ih, end_i)

                start_j = -p + j
                end_j = start_j + iw + 2*p - kw + 1
                actual_start_j = max(0, start_j)
                actual_end_j = min(iw, end_j)

                # print(start_i, end_i, '-', actual_start_i, actual_end_i)
                # print(start_j, end_j, '-', actual_start_j, actual_end_j)

                sparse_start_i = start_i + ((actual_start_i - start_i + sh-1) // sh) * sh 
                sparse_start_j = start_j + ((actual_start_j - start_j + sw-1) // sw) * sw

                # print(sparse_start_i, actual_end_i)
                # print(sparse_start_j, actual_end_j)

                inputs_slice = self.inputs.reshape(b, ih, iw, d, 1)[:, sparse_start_i:actual_end_i:sh, sparse_start_j:actual_end_j:sw, :]
                # print(inputs_slice.shape)

                o_start_i = (actual_start_i - start_i + sh - 1) // sh
                o_end_i = (actual_end_i - start_i + sh - 1) // sh
                o_start_j = (actual_start_j - start_j + sw - 1) // sw
                o_end_j = (actual_end_j - start_j + sw - 1) // sw

                # print(o_start_i, o_end_i)
                # print(o_start_j, o_end_j)

            
                output_grads_slice = output_grads.reshape(b, h, w, 1, c)[:, o_start_i:o_end_i, o_start_j:o_end_j, :, :]

                # print(output_grads_slice.shape)

                self.filters[i, j, :, :] -= (inputs_slice * output_grads_slice).sum(axis = (0, 1, 2)) * learning_rate / b

        return input_grads