#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main(int argc, char *argv[]) {
    Mat a = imread(argv[1]);
    Mat b;
    imshow("原图", a);
    Canny(a, b, 30, 100, 3);
    imshow("效果图", b);
    cvWaitKey(1000000);
    return 0;
}