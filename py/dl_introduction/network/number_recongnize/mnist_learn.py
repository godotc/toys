import numpy as np
import matplotlib.pyplot as plt
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Activation, Dropout
from keras.utils import to_categorical, plot_model


(x_train, y_train), (x_test, y_test) = mnist.load_data()
print(x_train.shape)
print(y_train.shape)


# count the number of unique train labels
unique, counts = np.unique(y_train, return_counts=True)
print("Train labels: ", dict(zip(unique, counts)))

# count the number of unique test labels
unique, counts = np.unique(y_test, return_counts=True)
print("Test labels: ", dict(zip(unique, counts)))


def show_samples():
    "sample 25 mnist digits from train dataset"
    indexs = np.random.randint(0, x_train.shape[0], size=25)
    images = x_train[indexs]
    labels = y_train[indexs]

    plt.figure(figsize=(5, 5))
    for i in range(len(indexs)):
        plt.subplot(5, 5, i+1)
        img = images[i]
        plt.imshow(img, cmap='gray')
        plt.axis('off')

    plt.show()
    plt.savefig("mnist-samples.png")
    plt.close('all')


# show_samples()

# Comput the number of labels
num_labels = len(np.unique(y_train))

# Cvt to one-hot vector
y_train = to_categorical(y_train)
y_test = to_categorical(y_test)

# img dimensions (assumed square)
img_size = x_train.shape[1]
input_size = img_size * img_size

# resized and normalize
x_train = np.reshape(x_train, [-1, input_size])
x_train = x_train.astype('float32') / 255
x_test = np.reshape(x_test, [-1, input_size])
x_test = x_test.astype('float32') / 255

# network parameter
batch_size = 128
hidden_uints = 256
dropout = 0.45


# model is a 3-layer MLP with ReLU and dropout after each layer
model = Sequential()

model.add(Dense(hidden_uints, input_dim=input_size))
model.add(Activation('relu'))
model.add(Dropout(dropout))

model.add(Dense(hidden_uints))
model.add(Activation('relu'))
model.add(Dropout(dropout))

model.add(Dense(num_labels))
model.add(Activation('softmax'))

model.summary()
