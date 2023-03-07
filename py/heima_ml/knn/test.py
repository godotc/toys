import numpy as np
import collections as c

sortedlabel = np.array([2, 2, 2, 3, 3, 1])
print(c.Counter(sortedlabel).most_common(1))  # (2,3) 2 occur 3 tims
print(c.Counter(sortedlabel).most_common(2))
print(c.Counter(sortedlabel).most_common(1)[0][0])  # most ele
