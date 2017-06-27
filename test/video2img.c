#include <cv.h>
#include <highgui.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    CvCapture *capture = cvCaptureFromAVI("~/Videos/Webcam/test.mp4");
    int i = 0;
    IplImage *img = 0;
    char image_name[25];
    cvNamedWindow("vivi");
    //读取和显示
    while (1) {
        img = cvQueryFrame(capture); //获取一帧图片
        if (img == NULL)
            break;

        cvShowImage("vivi", img); //将其显示
        char key = cvWaitKey(20);
        sprintf(image_name, "%s%d%s", "..\\tutu\\image", ++i,
                ".jpg");              //保存的图片名
        cvSaveImage(image_name, img); //保存一帧图片
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("vivi");

    return 0;
}