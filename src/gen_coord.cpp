#include <fstream>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const int limit = 200;
const int is_line = 100;

typedef struct xyz {
    int x;
    int y;
    int z;
} xyz;

void gen_coord(string file_path);

int main() {
    gen_coord("../data/img/sketch.jpg");
    return 0;
}

void gen_coord(string file_path) {
    Mat src = imread(file_path, 1);
    ofstream out("out.txt");
    int width = src.cols;
    int height = src.rows;
    int cp_img[height][width];
    bool tag[height][width];
    bool &map = tag;
    //初始化标签
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            tag[h][w] = 0;
        }
    }
}

xyz find_dot(xyz circle, bool &map, int width, int height) {
    int x = circle.x;
    int y = circle.y;
    xyz next_dot;
    int max_cr = max(max(max(x, y), abs(x - width - 1)), abs(y - height - 1));
    for (int cr = 1; cr < max_cr + 1; cr++) {       //分层找点
        for (int dn = -cr + 1; dn < cr + 1; dn++) { //下面的边
            if (y + cr + 1 > height)
                break;
            if (x + dn < 0)
                continue;
            if (x + dn + 1 > width)
                break;
            if (!map[x + dn][y + cr]) {
                next_dot = {x + dn, y + cr, cr};
                map[x + dn][y + cr] = 1;
                return next_dot;
            }
        }

        for (int rt = -cr + 1; rt < cr + 1; rt++) { //右面的边
            if (x + cr + 1 > width)
                break;
            if (y - rt + 1 > height)
                continue;
            if (y - rt < 0)
                break;
            if (!map[x + cr][y - rt]) {
                next_dot = {x + cr, y - rt, cr};
                map[x + cr][y - rt] = 1;
                return next_dot;
            }
        }

        for (int up = -cr + 1; up < cr + 1; up++) { //上面的边
            if (y - cr < 0)
                break;
            if (x - up + 1 > width)
                continue;
            if (x - up < 0)
                break;
            if (!map[x - up][y - cr]) {
                next_dot = {x - up, y - cr, cr};
                map[x - up][y - cr] = 1;
                return next_dot;
            }
        }

        for (int lt = -cr + 1; lt < cr + 1; lt++) { //左面的边
            if (x - cr < 0)
                break;
            if (y + lt < 0)
                continue;
            if (y + lt + 1 > height)
                break;
            if (!map[x - cr][y + lt]) {
                next_dot = {x - cr, y + lt, cr};
                map[x - cr][y + lt] = 1;
                return next_dot;
            }
        }
    }
    next_dot = {-1, -1, -1};
    return next_dot;
}

/*
    int direction = 0;
    int diff = 0;
    for (i = 1; i < height - 2; i += 2) {
        for (j = 1; j < width - 2; j += 2) {
            if (cp_img[i][j] > limit) { //颜色深度不够
                tag[i][j] = 1;
                continue;
            }
            diff0 = abs(cp_img[i][j] - cp_img[i + 1][j]);     //下
            diff1 = abs(cp_img[i][j] - cp_img[i + 1][j + 1]); //右下
            diff2 = abs(cp_img[i][j] - cp_img[i][j + 1]);     //右
            diff3 = abs(cp_img[i][j] - cp_img[i + 1][j - 1]); //左下
            diff = diff0;
            direction = 0;
            if (diff1 < diff) {
                diff = diff1;
                direction = 1;
            }
            if (diff2 < diff) {
                diff = diff2;
                direction = 2;
            }
            if (diff3 < diff) {
                diff = diff3;
                direction = 3;
            }

            if (diff > is_line) {
                tag[i][j] = 1;
                continue;
            }
            out << "(" << i << "," << j << ",0"
                << ")";
            switch (drection) {
            case 0： case 1:
            case 2:
            case 3:
            default:
            }
        }
    }

    */