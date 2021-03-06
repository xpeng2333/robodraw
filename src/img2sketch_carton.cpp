#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main(int argc, char *argv[])
{
    Mat src = imread("/home/xiaopeng/Desktop/robodraw/data/img/picture_carton.jpg", 1);
    int width = src.cols;
    int height = src.rows;
    //去色
    cvtColor(src, src, CV_BGR2GRAY);
    Canny(src, src, 50, 100, 3);
    //addWeighted(src, -1, NULL, 0, 255, src);
    imwrite("/home/xiaopeng/Desktop/robodraw/data/img/line.jpg", src);
    return 0;
}
