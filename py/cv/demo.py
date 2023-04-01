import numpy as np
import cv2

img = cv2.imread('./lambo.jpg')

gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)


cv2.imshow('Origin Image', img)
cv2.imshow('Gray Image', gray_img)
cv2.waitKey(0)


cv2.destroyAllWindows()
