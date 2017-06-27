# -*- coding: utf-8 -*-
import numpy as np
from PIL import Image
from pylab import *
# import types
from skimage import io, data

# 读取图片,灰度化，并转为数组
im0 = Image.open("../data/img/test.jpg").convert('L')
im = array(im0)

# print(abs(int(im[2, 8]) - int(im[3, 7])))

x = y = 0
m = im.shape[0]
n = im.shape[1]
h = range(m - 2)
k = range(n - 2)

# matrix = np.array([[255 for i in range(n)] for i in range(m)])
limit = 230

gray()

for x in h[::]:
    for y in k[::]:
        if(int(im[x, y]) > limit):
            im[x, y] = 255

imshow(im)

show()
