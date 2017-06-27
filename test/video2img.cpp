#include <iostream>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#pragma comment(lib, "ml.lib")
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "cvaux.lib")
#pragma comment(lib, "cvcam.lib")
#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "cxts.lib")
#pragma comment(lib, "highgui.lib")
#pragma comment(lib, "cvhaartraining.lib")

int main() {
    CvCapture *capture = NULL;
    IplImage *frame = NULL;
    char *AviFileName = "../data/video/test.mp4"; //视频的目录
    char *AviSavePath = "../data/img";            //图片保存的位置
    const int jiange = 2; //间隔两帧保存一次图片
    capture = cvCaptureFromCAM(0);
    //    capture = cvCaptureFromAVI(AviFileName);
    cvNamedWindow("MP4 player", 1);
    int count_tmp = 0; //计数总帧数
    char tmpfile[100] = {'\0'};
    while (true) {
        if (cvGrabFrame(capture)) {
            if (count_tmp % jiange == 0) {
                frame = cvRetrieveFrame(capture);
                cvShowImage("MP4 player", frame); //显示当前帧
                sprintf(tmpfile, "%s//%d.jpg", AviSavePath,
                        count_tmp); //使用帧号作为图片名
                cvSaveImage(tmpfile, frame);
            }
            if (cvWaitKey(10) >= 0) //延时
                break;
            ++count_tmp;
        } else {
            break;
        }
    }
    cvReleaseCapture(&capture);
    cvDestroyWindow("AVI player");
    std::cout << "总帧数" << count_tmp << std::endl;
    return 0;
}