# -*- coding: utf-8 -*-
import numpy as np
from PIL import Image
from pylab import *
# import types
from skimage import io, data

# 读取图片,灰度化，并转为数组
im0 = Image.open("../data/img/girl.jpeg").convert('L')
im = array(im0)

# print(abs(int(im[2, 8]) - int(im[3, 7])))

x = y = 0
m = im.shape[0]
n = im.shape[1]
h = range(m - 2)
k = range(n - 2)

matrix = np.array([[255 for i in range(n)] for i in range(m)])
limit = 255

gray()


# 将图像处理成“点图”，五点取样
for x in h[::2]:
    for y in k[::2]:
        im[x + 1, y + 1] = (int(im[x, y]) + int(im[x, y + 2]) + int(
            im[x + 1, y + 1]) + int(im[x + 2, y]) + int(im[x + 2, y + 2])) / 5
        im[x, y] = im[x, y + 1] = im[x + 1, y] = 255


# 确定划线方向函数,exist已存在0:无 1：左上 2：上 3：右上 4：左 x,y坐标
# 返回0:下 1：左下 2：右下 3：右
def direct(exist, x, y):
    right = abs(int(im[x, y]) - int(im[x, y + 1]))
    rightdown = abs(int(im[x, y]) - int(im[x + 1, y + 1]))
    down = abs(int(im[x, y]) - int(im[x + 1, y]))
    leftdown = abs(int(im[x, y]) - int(im[x + 1, y - 1]))
    sortlist = sorted([down, leftdown, rightdown, right])
    print(min(sortlist))
    return


# 在“点图”中连线
for x in h[:m - 2:2]:
    for y in k[3::2]:
        # 左上角已连线
        if(im[x - 1, y - 1] != 255):
            direct(1, x, y)
            continue

        # 上方已连线
        if(im[x - 1, y] != 255):
            direct(2, x, y)
            continue

        # 右上已连线
        if(im[x - 1, y + 1] != 255):
            direct(3, x, y)
            continue

        # 左方已连线
        if(im[x, y - 1] != 255):
            direct(4, x, y)
            continue

        # 孤立点
        direct(0, x, y)

imshow(im)
# imshow(matrix)
# io.imsave('dot.jpg', matrix)

# im0.save("test.jpg")

show()
