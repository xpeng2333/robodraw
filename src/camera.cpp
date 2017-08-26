#include <cstdio>
#include <highgui.h>
#include <string.h>

int main() {
    IplImage *pImg = NULL;
    CvCapture *cap = cvCaptureFromCAM(1);
    char filename[25] = "../data/img/cam2img.jpg";

    pImg = cvQueryFrame(cap);
    cvSaveImage(filename, pImg);
    cvXorS(pImg, cvScalarAll(255),
           pImg); //将拍到的图像反色（闪一下形成拍照效果）
    cvShowImage("Camera", pImg);
    cvWaitKey(200); //反色图像显示ms
    cvShowImage("Camera", pImg);

    cvReleaseCapture(&cap);
    return 0;
}
