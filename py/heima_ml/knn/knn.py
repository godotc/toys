# Your salary = average amount of your six best friends
import numpy as np
import collections as c

data = np.array([[154, 1], [126., 2], [70, 2], [196, 2], [161, 2], [371, 4]])

feature = data[:, 0]
label = data[:, -1]

predicPoint = 300

# per distance from throw point
distance = list(map(lambda x: abs(x-200), feature))
print(distance)

sortedindex = np.argsort(distance)
print(sortedindex)

sortedlabel = label[sortedindex]

# targets near to 200 (label), after sort,  2 of max 4 of min (by distance)
print(sortedlabel)

# most common
k = 3
print(c.Counter(sortedlabel[0:k]).most_common(1)[0][0])  # knn
