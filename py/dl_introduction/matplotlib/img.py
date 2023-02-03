import matplotlib.pyplot as plt
from matplotlib.image import imread

path = "./pics/微信图片_20221101210518.jpg"

img = imread(path)
plt.imshow(img)

plt.show()
