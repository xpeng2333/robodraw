# -*- coding: utf-8 -*-
import numpy as np
from PIL import Image
from pylab import *
#import types
from skimage import io, data

# 读取图片,灰度化，并转为数组
im0 = Image.open("../data/img/girl.jpeg").convert('L')
im = array(im0)

# print(type(im[1, 1]))

x = y = 0
m = im.shape[0]
n = im.shape[1]
h = range(m - 2)
k = range(n - 2)
matrix = np.array([[255 for i in range(n)] for i in range(m)])
limit = 255

gray()

# 将图像处理成“点图”
for x in h[::2]:
    for y in k[::2]:
        im[x + 1, y + 1] = (int(im[x, y]) + int(im[x, y + 2]) + int(
            im[x + 1, y + 1]) + int(im[x + 2, y]) + int(im[x + 2, y + 2])) / 5
        im[x, y] = 255

# 在“点图”中连线


imshow(im)
# imshow(matrix)
#io.imsave('dot.jpg', matrix)

# im0.save("test.jpg")

show()
