#include <iostream>
#include <opencv2/opencv.hpp>
// using namespace cv;
using namespace std;
int main() {
    cv::VideoCapture capture(0);
    cv::Mat edges;

    while (1) {
        cv::Mat frame;
        capture >> frame;
        cout << (int)*(frame.data) << endl;
        if (cv::waitKey(30) >= 0)
            break;
    }
    return 0;
}