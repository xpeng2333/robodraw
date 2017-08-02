#include <fstream>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const double PI = 3.1415926536;
const int angle2step = 120;

typedef struct angles {
    float a;
    float b;
    float r;
} angles;

typedef struct xyz {
    float x;
    float y;
    float z;
} xyz;

angles cal_angle(xyz coord);
string f_dirsteps(xyz curr, xyz next);
string num2str(int i);
void gen_coord(string file_path);
xyz find_dot(xyz circle, int *cp_img, int width, int height);

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

    for (int h = 0; h < height; h++) {
        uchar *P = src.ptr<uchar>(h);
        for (int w = 0; w < width; w++) {
            cp_img[h][w] = P[w];
        }
    }
    xyz center = {0, 0, 0};
    xyz next_center = {-1, -1, -1};
    int find_tag = 0;
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            if (cp_img[h][w] == 255) {
                center = {w, h, 0};
                find_tag = 1;
                break;
            }
        }
        if (find_tag == 1)
            break;
    }

    while (1) {
        next_center = find_dot(center, (int *)cp_img, width, height);
        if (next_center.z == -1)
            break;
        if (next_center.z > 3) {
            xyz temp1 = center;
            temp1.z = 1;
            f_dirsteps(center, temp1);
            next_center.z = 1;
            xyz temp2 = next_center;
            f_dirsteps(temp1, temp2);
            next_center.z = 0;
            f_dirsteps(temp2, next_center);
            center = next_center;
        } else {
            next_center.z = 0;
            f_dirsteps(center, next_center);
            center = next_center;
        }
        cp_img[int(center.y)][int(center.x)] = 0;
    }
    out.close();
}

xyz find_dot(xyz center, int *cp_img, int width, int height) {
    int x = center.x;
    int y = center.y;
    xyz next_dot;
    int map[height][width];
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            map[h][w] = *(cp_img + width * h + w);
        }
    }
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
                map[x + dn][y + cr] = 0;
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
                map[x + cr][y - rt] = 0;
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
                map[x - up][y - cr] = 0;
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
                map[x - cr][y + lt] = 0;
                return next_dot;
            }
        }
    }
    next_dot = {-1, -1, -1};
    return next_dot;
}

angles cal_angle(xyz coord) {
    float p = 0.0;
    angles angle;
    p = sqrt(coord.x * coord.x + coord.y * coord.y + coord.z * coord.z);
    if (coord.x <= 0)
        angle.a = asin(-coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) *
                  180 / PI;
    else
        angle.a = asin(coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) *
                      180 / PI +
                  180;

    coord.y = coord.y + 80 * sin(angle.a);
    coord.x = coord.x + 80 * cos(angle.a);
    p = sqrt(coord.x * coord.x + coord.y * coord.y + coord.z * coord.z);
    if (coord.x <= 0)
        angle.a = asin(-coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) *
                  180 / PI;
    else
        angle.a = asin(coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) *
                      180 / PI +
                  180;

    angle.b = (acos((45 / p) + (p / 500)) + asin(-coord.z / p)) * 180 / PI;
    angle.r =
        (acos((p * p - 22500) / (400 * p)) + acos(-coord.z / p)) * 180 / PI;

    return angle;
}

string f_dirsteps(xyz curr, xyz next) {
    string fpath = "$";
    angles curr_angles = cal_angle(curr);
    angles next_angles = cal_angle(next);
    float angle_a = next_angles.a - curr_angles.a;
    float angle_b = next_angles.b - curr_angles.b;
    float angle_r = next_angles.r - curr_angles.r;
    if (angle_a > 0)
        fpath = fpath + "1" + num2str(round(angle_a * angle2step)) + "#";
    else
        fpath = fpath + "0" + num2str(round(-angle_a * angle2step)) + "#";

    if (angle_b > 0)
        fpath = fpath + "1" + num2str(round(angle_b * angle2step)) + "#";
    else
        fpath = fpath + "0" + num2str(round(-angle_b * angle2step)) + "#";

    if (angle_r > 0)
        fpath = fpath + "1" + num2str(round(angle_r * angle2step)) + "#";
    else
        fpath = fpath + "0" + num2str(round(-angle_r * angle2step)) + "#";

    fpath = fpath + "@";
    return fpath;
}

string num2str(int i) {
    stringstream ss;
    ss << i;
    return ss.str();
}
