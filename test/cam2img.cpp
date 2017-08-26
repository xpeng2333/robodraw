#include <cmath>
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
    VideoCapture capture(1);
    Mat frame;
    capture >> frame;
    Mat canvas(frame.rows, frame.cols, CV_8UC3, Scalar(255, 255, 255));
    int position_startx, position_starty;
    int position_endx, position_endy = -1;
    bool first_tag = false;
    int limit = 20;

    while (1) {
        capture >> frame;
        int rowNum = frame.rows;
        int colNum = frame.cols * frame.channels();
        for (int i = 0; i < rowNum; i++) {
            uchar *data = frame.ptr<uchar>(i);
            bool find = false;
            for (int j = 0; j < colNum; j += 3) {
                if (limit > max(max(abs(data[j] - 10), abs(data[j + 1] - 10)),
                                abs(data[j + 2] - 10))) {
                    if (first_tag == true) {
                        position_startx = position_endx;
                        position_starty = position_endy;
                        position_endx = j;
                        position_endy = i;
                    } else {
                        position_startx = position_endx = j;
                        position_starty = position_endy = i;
                        first_tag = true;
                    }
                    find = true;
                    break;
                }
                /*
                                if (data[j] > -1 && data[j] < 30) {
                                    if (data[j + 1] > -1 && data[j + 1] < 30) {
                                        if (data[j + 2] > -1 && data[j + 2] <
                   30) {
                                            if (first_tag == true) {
                                                position_startx = position_endx;
                                                position_starty = position_endy;
                                                position_endx = j;
                                                position_endy = i;
                                            } else {
                                                position_startx = position_endx
                   = j;
                                                position_starty = position_endy
                   = i;
                                                first_tag = true;
                                            }
                                            find = true;
                                            break;
                                        }
                                    }
                                }
                */
            }
            if (find)
                break;
        }
        if (position_endy != -1) {
            printf("%d %d\n", position_endx, position_endy);
            DrawLine(canvas, Point(position_startx, position_starty),
                     Point(position_endx, position_endy));
        }

        imshow("画布", canvas);
        waitKey(30);
        usleep(1000000);
        printf("I am awake\n");
    }

    return 0;
}

void DrawLine(Mat img, Point start, Point end) {
    int thickness = 2;
    int lineType = 8;
    line(img, start, end, Scalar(0, 0, 0), thickness, lineType);
}