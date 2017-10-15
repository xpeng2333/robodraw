#-*-coding:utf-8-*-
import os
import time

try:
    mPic_time0 = time.ctime(os.stat("../data/img/picture.jpg").st_mtime)
    mPicton_time0 = time.ctime(
        os.stat("../data/img/picture_carton.jpg").st_mtime)
    line0 = time.ctime(
        os.stat("../data/img/line.jpg").st_mtime)
    step0 = time.ctime(
        os.stat("../data/text/out_step.txt").st_mtime)
except:
    pass
while True:
    time.sleep(0.5)
    try:
        mPic_time1 = time.ctime(os.stat("../data/img/picture.jpg").st_mtime)
        mPicton_time1 = time.ctime(
            os.stat("../data/img/picture_carton.jpg").st_mtime)
        line1 = time.ctime(
            os.stat("../data/img/line.jpg").st_mtime)
        step1 = time.ctime(os.stat("../data/text/out_step.txt").st_mtime)
    except:
        pass
    if(mPic_time0 != mPic_time1):
        print("图片已更新")
        mPic_time0 = mPic_time1
        os.system("./img2sketch_carton")
        time.sleep(1)
    if(mPicton_time0 != mPicton_time1):
        print("卡通图片已更新")
        mPicton_time0 = mPicton_time1
        os.system("./img2sketch_carton")
        time.sleep(1)
    if(line0 != line1):
        print("线条画已更新")
        line0 = line1
        os.system("./gen_coord")
        time.sleep(1)
    if(step0 != step1):
        print("电机驱动文件已更新")
        step0 = step1
        os.system("python send_PC.py")
        time.sleep(1)
