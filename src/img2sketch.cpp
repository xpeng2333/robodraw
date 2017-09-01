#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main(int argc, char *argv[]) {
    Mat src = imread("../data/img/cam2img.jpg", 1);
    int width = src.cols;
    int height = src.rows;
    Mat gray0;
    //去色
    cvtColor(src, gray0, CV_BGR2GRAY);
    imwrite("../data/img/gray0.jpg", gray0);
    //融合：颜色减淡
    /*
    Mat img(gray0.size(), CV_8UC1);
    Mat img_line(gray0.size(), CV_8UC1);
      for (int y = 0; y < height; y++) {

          uchar *P0 = gray0.ptr<uchar>(y);
          uchar *P1 = gray1.ptr<uchar>(y);
          uchar *P = img.ptr<uchar>(y);
          for (int x = 0; x < width; x++) {
              int tmp0 = P0[x];
              int tmp1 = P1[x];
              P[x] = (uchar)min((tmp0 + (tmp0 * tmp1) / (256 - tmp1)), 255);
          }
      }
      */
    // Canny(img, img_line, 90, 100, 3);
    // addWeighted(img_line, -1, NULL, 0, 255, img_line);
    Mat high_color(gray0.size(), CV_8UC1);
    Mat low_color(gray0.size(), CV_8UC1);
    for (int y = 0; y < height; y++) {
        uchar *P0 = low_color.ptr<uchar>(y);
        uchar *P1 = high_color.ptr<uchar>(y);
        uchar *P = gray0.ptr<uchar>(y);
        for (int x = 0; x < width; x++) {
            if (P[x] < 80)
                P1[x] = P[x];
            else
                P0[x] = P[x];
        }
    }
    // imwrite("../data/img/sketch.jpg", img);
    // imwrite("../data/img/line.jpg", img_line);
    // GaussianBlur(low_color, low_color, Size(21, 21), 0);
    // GaussianBlur(low_color, low_color, Size(61, 61), 0);
    imwrite("../data/img/low0.jpg", low_color);
    imwrite("../data/img/high0.jpg", high_color);
    Canny(low_color, low_color, 50, 120, 3);
    Canny(high_color, high_color, 50, 100, 3);
    for (int y = 0; y < height; y++) {
        uchar *P0 = low_color.ptr<uchar>(y);
        uchar *P1 = high_color.ptr<uchar>(y);
        uchar *P = gray0.ptr<uchar>(y);
        for (int x = 0; x < width; x++) {
            P[x] = P0[x] + P1[x];
        }
    }
    addWeighted(gray0, -1, NULL, 0, 255, gray0);
    imwrite("../data/img/low1.jpg", low_color);
    imwrite("../data/img/high1.jpg", high_color);
    imwrite("../data/img/line.jpg", gray0);
    return 0;
}