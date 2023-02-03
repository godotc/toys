import sys
import os
import numpy as np
from matplotlib import pyplot as plt
from keras.datasets import mnist
from keras.layers import Dense, Activation, Dropout
import pickle
if sys.path.append(os.path.pardir):
    import ut


def img_show(img):
    plt.imshow(img, cmap=plt.get_cmap('gray'))
    plt.show()


# (x_train, t_train), (x_test, t_test) = mnist.load_data()
# print(x_train.shape)
# print(t_train.shape)
# print(x_test.shape)
# print(t_test.shape)

# img = x_train[0]
# label = t_train[0]
# print(label)

# img_show(img)


def get_data():
    (x_train, t_train), (x_test, t_test) = mnist.load_data()
    return x_test, t_test


def init_network():
    with open("sample_weight.pkl", 'rb') as f:
        network = pickle.load(f)
    return network


def predict(network, x):
    W1, W2, W3 = network['W1'], network['W2'], network['W3']
    b1, b2, b3 = network['b1'], network['b2'], network['b3']

    a1 = np.dot(x, W1) + b1
    z1 = ut.sigmoid(a1)
    a2 = np.dot(z1, W2) + b2
    z2 = ut.sigmoid(a2)
    a3 = np.dot(z2, W3) + b3
    y = ut.softmax(a3)

    return y


x, t = get_data()
network = init_network

accuracy_cnt = 0
for i in range(len(x)):
    y = predict(network, x[i])
    p = np.argmax(y)  # get max probability
    if p == t[i]:
        accuracy_cnt += 1

print("Accuracy: " + str(float(accuracy_cnt)/len(x)))
