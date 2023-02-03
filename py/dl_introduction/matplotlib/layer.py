import numpy as np
import matplotlib.pylab as plt


def step_func(x):
    return np.array(x > 0, dtype=np.int_)


def sigmoid(x):
    return 1/(1+np.exp(-x))


def test_step():
    x = np.arange(-5.0, 5.0, 0.1)
    y = step_func(x)
    plt.plot(x, y)
    plt.ylim(-0.1, 1.1)  # Configure y axis range
    plt.show()


def test_sigmoid():
    x = np.arange(-5., 5., 0.1)
    y = sigmoid(x)
    plt.plot(x, y)
    plt.ylim(-0.1, 1.1)  # Configure y axis range
    plt.show()


# test_sigmoid()

"""
  first layer is 1 row 2 col, so output is 1 row
  second layer 2 row 3 col, so output is  3 col
  --> 1 row 3 col to next layer
"""
X = np.array([1.0, 0.5])
W1 = np.array([[0.1, 0.3, 0.5], [0.2, 0.4, 0.6]])
B1 = np.array([0.1, 0.2, 0.3])
#
A1 = np.dot(X, W1) + B1
Z1 = sigmoid(A1)

print(A1)
print(Z1)


# 3 row 2 col --> 1 row 2 col
W2 = np.array([[0.1, 0.4], [0.2, 0.5], [0.3, 0.6]])
B2 = np.array([0.1, 0.2])
#
A2 = np.dot(Z1, W2)+B2
Z2 = sigmoid(A2)
print(A2)
print(Z2)
