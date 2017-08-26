#include <fstream>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const double PI = 3.1415926536;
const float angle2step = 6875.4935;
const float std_z = -270;

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
xyz convert_coord(xyz origin_coord, int width, int height);

xyz init_coord = {280, 0, -250};

int main() {
    gen_coord("../data/img/line.jpg");
    return 0;
}

void gen_coord(string file_path) {
    Mat src = imread(file_path, 0);
    ofstream out("../data/text/out_step.txt");
    int width = src.cols;
    int height = src.rows;
    int cp_img[height][width];
    cout << width << " " << height << endl;
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
            if (cp_img[h][w] > 240) {
                center = {w, h, 0};
                cp_img[h][w] = 0;
                find_tag = 1;
                break;
            }
        }
        if (find_tag == 1)
            break;
    }

    out << f_dirsteps(init_coord, convert_coord(center, width, height));
    center.z = 1;
    while (1) {
        next_center = find_dot(center, *cp_img, width, height);
        // cout << next_center.x << " " << next_center.y << " " << next_center.z
        //    << endl;
        if (int(next_center.z) == -1)
            break;

        if (int(next_center.z) > 30) {
            xyz temp1 = center;
            temp1.z = 0;
            out << f_dirsteps(convert_coord(center, width, height),
                              convert_coord(temp1, width, height));
            next_center.z = 0;
            xyz temp2 = next_center;
            out << f_dirsteps(convert_coord(temp1, width, height),
                              convert_coord(temp2, width, height));
            next_center.z = 1;
            out << f_dirsteps(convert_coord(temp2, width, height),
                              convert_coord(next_center, width, height));
        } else {
            next_center.z = 1;
            out << f_dirsteps(convert_coord(center, width, height),
                              convert_coord(next_center, width, height));
        }
        cp_img[int(center.y)][int(center.x)] = 0;
        center = next_center;
    }
    next_center = center;
    next_center.z = 0;
    out << f_dirsteps(convert_coord(center, width, height),
                      convert_coord(next_center, width, height));
    out << f_dirsteps(convert_coord(next_center, width, height), init_coord);
    out.close();
}

xyz find_dot(xyz center, int *cp_img, int width, int height) {
    int x = center.x;
    int y = center.y;
    xyz next_dot;

    int max_cr = max(max(max(x, y), abs(x - width + 1)), abs(y - height + 1));
    for (int cr = 1; cr < max_cr + 1; cr++) {       //分层找点
        for (int dn = -cr + 1; dn < cr + 1; dn++) { //下面的边
            if (y + cr + 1 > height)
                break;
            if (x + dn < 0)
                continue;
            if (x + dn + 1 > width)
                break;

            if (*(cp_img + width * (y + cr) + x + dn) > 245) {
                next_dot = {x + dn, y + cr, cr};
                *(cp_img + width * (y + cr) + x + dn) = 0;
                return next_dot;
            }
        }

        for (int rt = -cr + 1; rt < cr + 1; rt++) { //右面的边
            if (x + cr + 1 > width)
                break;
            if (y - rt + 2 > height)
                continue;
            if (y - rt < 0)
                break;

            if (*(cp_img + width * (y - rt) + x + cr) > 245) {
                next_dot = {x + cr, y - rt, cr};
                *(cp_img + width * (y - rt) + x + cr) = 0;
                return next_dot;
            }
        }

        for (int up = -cr + 1; up < cr + 1; up++) { //上面的边
            if (y - cr < 0)
                break;
            if (x - up + 2 > width)
                continue;
            if (x - up < 0)
                break;

            if (*(cp_img + width * (y - cr) + x - up) > 245) {
                next_dot = {x - up, y - cr, cr};
                *(cp_img + width * (y - cr) + x - up) = 0;
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

            if (*(cp_img + width * (y + lt) + x - cr) > 245) {
                next_dot = {x - cr, y + lt, cr};
                *(cp_img + width * (y + lt) + x - cr) = 0;
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
    if (coord.x >= 0)
        angle.a = asin(coord.y / sqrt(coord.x * coord.x + coord.y * coord.y));
    else
        angle.a =
            asin(-coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) - PI;

    coord.y = coord.y - 80 * sin(angle.a);
    coord.x = coord.x - 80 * cos(angle.a);
    p = sqrt(coord.x * coord.x + coord.y * coord.y + coord.z * coord.z);
    angle.b = (acos((45 / p) + (p / 500)) + asin(-coord.z / p));
    angle.r = (acos((p * p - 22500) / (400 * p)) + acos(-coord.z / p));

    return angle;
}

string f_dirsteps(xyz curr, xyz next) {
    string fpath = "$";
    //  cout << curr.x << " " << curr.y << " " << curr.z << endl;
    angles curr_angles = cal_angle(curr);
    angles next_angles = cal_angle(next);
    float angle_a = next_angles.a - curr_angles.a;
    float angle_b = next_angles.b - curr_angles.b;
    float angle_r = next_angles.r - curr_angles.r;

    int a_step = round(angle_a * angle2step);
    int b_step = round(angle_b * angle2step);
    int r_step = round(angle_r * angle2step);

    if (!(a_step || b_step || r_step))
        return "";
    if (angle_a >= 0)
        fpath = fpath + "1" + num2str(a_step) + "#";
    else
        fpath = fpath + "0" + num2str(-a_step) + "#";

    if (angle_b >= 0)
        fpath = fpath + "1" + num2str(b_step) + "#";
    else
        fpath = fpath + "0" + num2str(-b_step) + "#";

    if (angle_r >= 0)
        fpath = fpath + "1" + num2str(r_step) + "#";
    else
        fpath = fpath + "0" + num2str(-r_step) + "#";

    fpath = fpath + "@";
    return fpath;
}

string num2str(int i) {
    stringstream ss;
    ss << i;
    return ss.str();
}

xyz convert_coord(xyz origin_coord, int width, int height) {
    xyz cvt_coord;
    int std_width;
    int std_height;
    float ratio;
    if (width >= height) {
        std_width = 7 * width;
        std_height = 10 * height;
        if (std_width < std_height) {
            ratio = 210.0 / height;
            cvt_coord.x = 330 - int(origin_coord.y * ratio);
            cvt_coord.y = (50 - (300 - int(width * ratio)) / 2) -
                          int(origin_coord.x * ratio);
            cvt_coord.z = std_z - 10 * origin_coord.z;
        } else {
            ratio = 300.0 / width;
            cvt_coord.x = (330 - (210 - int(height * ratio)) / 2) -
                          int(origin_coord.y * ratio);
            cvt_coord.y = 50 - int(origin_coord.x * ratio);
            cvt_coord.z = std_z - 10 * origin_coord.z;
        }
    } else {
        std_width = 10 * width;
        std_height = 7 * height;
        if (std_width < std_height) {
            ratio = 300.0 / height;
            cvt_coord.x = (120 + (210 - int(width * ratio)) / 2) +
                          int(origin_coord.x * ratio);
            cvt_coord.y = 50 - int(origin_coord.y * ratio);
            cvt_coord.z = std_z - 10 * origin_coord.z;
        } else {
            ratio = 210.0 / width;
            cvt_coord.x = 120 + int(origin_coord.x * ratio);
            cvt_coord.y = (50 - (300 - int(height * ratio)) / 2) -
                          int(origin_coord.y * ratio);
            cvt_coord.z = std_z - 10 * origin_coord.z;
        }
    }
    return cvt_coord;
}