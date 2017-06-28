#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <unistd.h>

using namespace cv;
using namespace std;

void DrawLine(Mat img, Point start, Point end);

int main() {
    VideoCapture capture(0);
    Mat frame;
    capture >> frame;
    Mat canvas(frame.rows, frame.cols, CV_8UC3, Scalar(255, 255, 255));
    int position_startx, position_starty = -1;
    int position_endx, position_endy = 0;
    while (1) {
        capture >> frame;
        int rowNum = frame.rows;
        int colNum = frame.cols;
        for (int i = 0; i < rowNum; i++) {
            uchar *data = frame.ptr<uchar>(i);
            for (int j = 0; j < colNum; j++) {
                if (data[j] > 0 && data[j] < 30)
                    if (data[j + 1] > 100 && data[j + 1] < 200)
                        if (data[j + 2] > 250 && data[j + 2] < 256) {
                            position_startx = position_endx;
                            position_starty = position_endy;
                            position_endx = i;
                            position_endy = j;
                        }
                if (position_starty != -1)
                    DrawLine(canvas, Point(position_startx, position_starty),
                             Point(position_endx, position_endy));
                imshow("画布", canvas);
                waitKey(30);
            }
        }

        usleep(100000);
    }

    return 0;
}

void DrawLine(Mat img, Point start, Point end) {
    int thickness = 2;
    int lineType = 8;
    line(img, start, end, Scalar(0, 0, 0), thickness, lineType);
}