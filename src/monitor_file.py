import os
import time

try:
    mPic_time0 = time.ctime(os.stat("../data/img/picture.jpg").st_mtime)
    mPicton_time0 = time.ctime(
        os.stat("../data/img/picture_carton.jpg").st_mtime)
except:
    pass
while True:
    time.sleep(1)
    try:
        mPic_time1 = time.ctime(os.stat("../data/img/picture.jpg").st_mtime)
        mPicton_time1 = time.ctime(
            os.stat("../data/img/picture_carton.jpg").st_mtime)
    except:
        pass
    if(mPic_time0 != mPic_time1):
        mPic_time0 = mPic_time1
        os.system("./img2sketch")
        os.system("python send_PC.py")
    if(mPicton_time0 != mPicton_time1):
        mPicton_time0 = mPicton_time1
        os.system("./img2sketch_carton")
        os.system("python send_PC.py")
