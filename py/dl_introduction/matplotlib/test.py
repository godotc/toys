import PyQt6
import numpy as np
import matplotlib.pyplot as plt
plt.matplotlib.use('QtAgg')


x = np.arange(0, 6, 0.1)
y = np.sin(x)

plt.plot(x, y)
plt.show()
