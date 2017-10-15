#include <fstream>
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

const double PI = 3.1415926536;
const double angle2step = 6875.4935;
const double std_z = -294;
const double std_color = 140;

long a1 = 0;
long a2 = 0;
long a3 = 0;
long total = 0;

typedef struct angles
{
    double a;
    double b;
    double r;
} angles;

typedef struct xyz
{
    double x;
    double y;
    double z;
} xyz;

angles cal_angle(xyz coord);
string f_dirsteps(xyz curr, xyz next);
string num2str(long i);
void gen_coord(string file_path);
xyz find_dot(xyz circle, long *cp_img, long width, long height);
xyz convert_coord(xyz origin_coord, long width, long height);

xyz init_coord = {245, -5, -250};

int main()
{
    gen_coord("../data/img/line.jpg");
    printf("%d %d %d\n", a1, a2, a3);
    printf("%d min\n", (total / (1000000 / 800)) / 60);
    return 0;
}

void gen_coord(string file_path)
{
    Mat src = imread(file_path, 0);
    ofstream out("../data/text/out_step.txt");
    long width = src.cols;
    long height = src.rows;
    long m = std::min(width, height);
    long cp_img[height][width];
    for (long h = 0; h < height; h++)
    {
        uchar *P = src.ptr<uchar>(h);
        for (long w = 0; w < width; w++)
        {
            cp_img[h][w] = P[w];
        }
    }
    xyz center = {0, 0, 0};
    xyz next_center = {-1, -1, -1};
    long find_tag = 0;
    for (long h = 0; h < height; h++)
    {
        for (long w = 0; w < width; w++)
        {
            if (cp_img[h][w] > std_color)
            {
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
    xyz init_down = center;
    init_down.z = 1;
    out << f_dirsteps(convert_coord(center, width, height), convert_coord(init_down, width, height));
    center.z = 1;
    while (1)
    {
        next_center = find_dot(center, *cp_img, width, height);
        if (long(next_center.z) == -1)
            break;

        if (long(next_center.z) > (m / 50))
        {
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
        }
        else
        {
            next_center.z = 1;
            out << f_dirsteps(convert_coord(center, width, height),
                              convert_coord(next_center, width, height));
        }
        cp_img[long(center.y)][long(center.x)] = 0;
        center = next_center;
    }
    next_center = center;
    next_center.z = 0;
    out << f_dirsteps(convert_coord(center, width, height),
                      convert_coord(next_center, width, height));
    out << f_dirsteps(convert_coord(next_center, width, height), init_coord);
    out.close();
}

xyz find_dot(xyz center, long *cp_img, long width, long height)
{
    long x = (long)center.x;
    long y = (long)center.y;
    xyz next_dot;

    long max_cr = max(max(max(x, y), abs(x - width + 1)), abs(y - height + 1));
    for (long cr = 1; cr < max_cr + 1; cr++)
    { //分层找点
        for (long dn = -cr + 1; dn < cr + 1; dn++)
        { //下面的边
            if (y + cr + 1 > height)
                break;
            if (x + dn < 0)
                continue;
            if (x + dn + 1 > width)
                break;

            if (*(cp_img + width * (y + cr) + x + dn) > std_color)
            {
                next_dot = {x + dn, y + cr, cr};
                *(cp_img + width * (y + cr) + x + dn) = 0;
                return next_dot;
            }
        }

        for (long rt = -cr + 1; rt < cr + 1; rt++)
        { //右面的边
            if (x + cr + 1 > width)
                break;
            if (y - rt + 2 > height)
                continue;
            if (y - rt < 0)
                break;

            if (*(cp_img + width * (y - rt) + x + cr) > std_color)
            {
                next_dot = {x + cr, y - rt, cr};
                *(cp_img + width * (y - rt) + x + cr) = 0;
                return next_dot;
            }
        }

        for (long up = -cr + 1; up < cr + 1; up++)
        { //上面的边
            if (y - cr < 0)
                break;
            if (x - up + 2 > width)
                continue;
            if (x - up < 0)
                break;

            if (*(cp_img + width * (y - cr) + x - up) > std_color)
            {
                next_dot = {x - up, y - cr, cr};
                *(cp_img + width * (y - cr) + x - up) = 0;
                return next_dot;
            }
        }

        for (long lt = -cr + 1; lt < cr + 1; lt++)
        { //左面的边
            if (x - cr < 0)
                break;
            if (y + lt < 0)
                continue;
            if (y + lt + 1 > height)
                break;

            if (*(cp_img + width * (y + lt) + x - cr) > std_color)
            {
                next_dot = {x - cr, y + lt, cr};
                *(cp_img + width * (y + lt) + x - cr) = 0;
                return next_dot;
            }
        }
    }
    next_dot = {-1, -1, -1};
    return next_dot;
}

angles cal_angle(xyz coord)
{
    double p = 0.0;
    angles angle;
    if (coord.x >= 0)
        angle.a = asin(coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) +
                  asin(5 / sqrt(coord.x * coord.x + coord.y * coord.y));
    else
        angle.a = asin(-coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) -
                  PI + asin(5 / sqrt(coord.x * coord.x + coord.y * coord.y));

    coord.x = coord.x - 45 * cos(angle.a) - 5 * sin(angle.a);
    coord.y = coord.y - 45 * sin(angle.a) + 5 * cos(angle.a);
    p = sqrt(coord.x * coord.x + coord.y * coord.y + coord.z * coord.z);
    angle.b = (acos((45 / p) + (p / 500)) + asin(-coord.z / p));
    angle.r = (acos((p * p - 22500) / (400 * p)) + acos(-coord.z / p));
    return angle;
}

string f_dirsteps(xyz curr, xyz next)
{
    string fpath = "$";

    angles curr_angles = cal_angle(curr);
    angles next_angles = cal_angle(next);

    double angle_a = next_angles.a - curr_angles.a;
    double angle_b = next_angles.b - curr_angles.b;
    double angle_r = next_angles.r - curr_angles.r;

    long a_step = round(next_angles.a * angle2step) - round(curr_angles.a * angle2step);
    long b_step = round(next_angles.b * angle2step) - round(curr_angles.b * angle2step);
    long r_step = round(next_angles.r * angle2step) - round(curr_angles.r * angle2step);

    a1 += a_step;
    a2 += b_step;
    a3 += r_step;

    total += max(abs(a_step), max(abs(b_step), abs(r_step)));

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

    if (r_step >= 0)
        fpath = fpath + "1" + num2str(r_step) + "#";
    else
        fpath = fpath + "0" + num2str(-r_step) + "#";

    fpath = fpath + "@";
    return fpath;
}

string num2str(long i)
{
    stringstream ss;
    ss << i;
    return ss.str();
}

xyz convert_coord(xyz origin_coord, long width, long height)
{
    xyz cvt_coord;
    long std_width;
    long std_height;
    double ratio;
    if (width >= height)
    {
        std_width = 7 * width;
        std_height = 10 * height;
        if (std_width < std_height)
        {
            ratio = 210.0 / height;
            cvt_coord.x = 305 - (origin_coord.y * ratio);
            cvt_coord.y = (100 - (300 - (width * ratio)) / 2) -
                          (origin_coord.x * ratio);
            cvt_coord.z = std_z - 20 * origin_coord.z;
        }
        else
        {
            ratio = 300.0 / width;
            cvt_coord.x = (305 - (210 - (height * ratio)) / 2) -
                          (origin_coord.y * ratio);
            cvt_coord.y = 100 - (origin_coord.x * ratio);
            cvt_coord.z = std_z - 20 * origin_coord.z;
        }
    }
    else
    {
        std_width = 10 * width;
        std_height = 7 * height;
        if (std_width < std_height)
        {
            ratio = 300.0 / height;
            cvt_coord.x = (95 + (210 - (width * ratio)) / 2) +
                          (origin_coord.x * ratio);
            cvt_coord.y = 100 - (origin_coord.y * ratio);
            cvt_coord.z = std_z - 20 * origin_coord.z;
        }
        else
        {
            ratio = 210.0 / width;
            cvt_coord.x = 95 + (origin_coord.x * ratio);
            cvt_coord.y = (100 - (300 - (height * ratio)) / 2) -
                          (origin_coord.y * ratio);
            cvt_coord.z = std_z - 20 * origin_coord.z;
        }
    }
    return cvt_coord;
}