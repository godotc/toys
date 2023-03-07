import numpy as np
import collections as c


def knn(k, predicPoint, feature, label):
    feature = list(map(lambda x: abs(x-predicPoint), feature))
    sortedindex = np.argsort(feature)
    sortedlabel = label[sortedindex]
    return c.Counter(sortedlabel).most_common(k)[0][0]


if __name__ == "__main__":
    data = np.load("1.csv")

    predictpoint = 300
    k = 3
    knn(k, predictpoint, data, None)
