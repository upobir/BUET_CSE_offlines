1. first trial (forgottten architecture), `first_test.pkl`. __validation loss__ = 23%. rate = 0.0005, 100 epochs, 1000 data. 6 hours
2. `second_test.pkl`, __validation loss__ = 19%, rate = 0.0025/0.0005, 20 epochs, 3500 data. Overfitted, 1 hour 20 mins
```
cnn.addLayer(ConvolutionLayer(10, (4, 4), (2, 2), 0))
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(ConvolutionLayer(8, (3, 3), (1, 1), 0))
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(ConvolutionLayer(6, (2, 2), (1, 1), 0))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(FlatteningLayer())
cnn.addLayer(DenseLayer(30))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(DenseLayer(10))
cnn.addLayer(SoftMaxLayer())
```
3. `third_test.pkl`, same as above, but preprocessed image with dilation. __validation loss__ = 11%, rate = 0.003, 30 epochs, 4000 data. didn't learn
4. `fourth_test.pkl`, this uses 64 * 64 images, with grayscale channel and scaled to make max value 1, __validation loss__ = 55% so far, rate = 0.005, 200 epochs, 6000 data, learning good
```
cnn.addLayer(ConvolutionLayer(10, (4, 4), (2, 2), 0))
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(ConvolutionLayer(5, (3, 3), (2, 2), 0))
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(ConvolutionLayer(3, (3, 3), (2, 2), 0))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(FlatteningLayer())
cnn.addLayer(DenseLayer(30))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(DenseLayer(10))
cnn.addLayer(SoftMaxLayer())
```
5. `fifth_test.pkl`, dilation now done first, __validation loss__ = 45%, rate = 0.004, 50 epochs, 15000 
```
cnn.addLayer(ConvolutionLayer(3, (4, 4), (2, 2), 0))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ConvolutionLayer(3, (3, 3), (2, 2), 1))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ConvolutionLayer(1, (2, 2), (2, 2), 1))
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(FlatteningLayer())
cnn.addLayer(DenseLayer(20))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(DenseLayer(10))
cnn.addLayer(SoftMaxLayer())
```
6. `sixth_test.pkl`, 48*48 size, __validation loss__ = 19%, rate = 0.001, 50 epochs, 15000
```
cnn.addLayer(ConvolutionLayer(3, (5, 5), (2, 2), 4))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(MaxPoolingLayer((3, 2), (1, 1)))
cnn.addLayer(ConvolutionLayer(2, (2, 2), (2, 2), 0))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(MaxPoolingLayer((3, 3), (1, 1)))
cnn.addLayer(ConvolutionLayer(1, (3, 3), (2, 2), 1))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(FlatteningLayer())
cnn.addLayer(DenseLayer(18))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(DenseLayer(10))
cnn.addLayer(SoftMaxLayer())
```
7. `seventh_test.pkl`, 64*64 size, __validation loss__ = 20%, rate = 0.003, 50 epochs, 15000
```
cnn.addLayer(ConvolutionLayer(10, (3, 3), (2, 2), 0))
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(ConvolutionLayer(5, (3, 3), (2, 2), 1))
cnn.addLayer(MaxPoolingLayer((2, 2), (1, 1)))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(ConvolutionLayer(3, (3, 3), (2, 2), 1))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(FlatteningLayer())
cnn.addLayer(DenseLayer(40))
cnn.addLayer(ReLUActivationLayer())
cnn.addLayer(DenseLayer(10))
cnn.addLayer(SoftMaxLayer())
```
8. `eighth_test.pkl`, 64*64 size, recreation of fourht one, __validation loss__ = 40%, rate = 0.003, 100 epochs, 15000