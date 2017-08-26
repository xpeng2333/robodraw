# -*- coding: utf-8 -*-
import numpy as np
from PIL import Image
from pylab import *
# import types
from skimage import io, data

# 读取图片,灰度化，并转为数组
im0 = Image.open("../data/img/sketch.jpg")
im = array(im0)


imshow(im)
# imshow(matrix)
# io.imsave('dot.jpg', matrix)

# im0.save("test.jpg")

show()
