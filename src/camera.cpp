#include <cstdio>
#include <highgui.h>
#include <string.h>

int main()
{
    IplImage *pImg = NULL;
    CvCapture *cap = cvCaptureFromCAM(0);
    char filename[25] = "../data/img/picture.jpg";
    char key;

    while (1)
    {
        pImg = cvQueryFrame(cap);
        cvFlip(pImg, NULL,
               1); //水平翻转图像，像照镜子一样，不想要这个效果可以去掉此句
        key = cvWaitKey(50);
        if (key == 27)
            break;      //按ESC键退出程序
        if (key == 'c') //按c键拍照
        {
            cvSaveImage(filename, pImg);
            cvXorS(pImg, cvScalarAll(255),
                   pImg); //将拍到的图像反色（闪一下形成拍照效果）
            cvShowImage("Camera", pImg);
            cvWaitKey(200); //反色图像显示ms
        }
        cvShowImage("Camera", pImg);
    }
    cvReleaseCapture(&cap);
    return 0;
}
