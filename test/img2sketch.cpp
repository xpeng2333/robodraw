#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main(int argc, char *argv[]) {
    Mat src = imread(argv[1], 1);
    int width = src.cols;
    int heigh = src.rows;
    Mat gray0, gray1;
    //去色
    cvtColor(src, gray0, CV_BGR2GRAY);
    //反色
    addWeighted(gray0, -1, NULL, 0, 255, gray1);
    //高斯模糊,高斯核的Size与最后的效果有关
    GaussianBlur(gray1, gray1, Size(21, 21), 0);

    //融合：颜色减淡
    Mat img(gray1.size(), CV_8UC1);
    for (int y = 0; y < heigh; y++) {

        uchar *P0 = gray0.ptr<uchar>(y);
        uchar *P1 = gray1.ptr<uchar>(y);
        uchar *P = img.ptr<uchar>(y);
        for (int x = 0; x < width; x++) {
            int tmp0 = P0[x];
            int tmp1 = P1[x];
            P[x] = (uchar)min((tmp0 + (tmp0 * tmp1) / (256 - tmp1)), 255);
        }
    }
    //    imshow("素描", img);
    waitKey();
    imwrite("sketch.jpg", img);
}