#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

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

using namespace std;

angles cal_angle(xyz coord);
string f_dirsteps(xyz curr, xyz next);
string num2str(int i);

int main() {
    int num = 2 * PI / 0.05;
    float an = 3;
    ofstream out("out.txt");
    xyz curr = {200, 0, -250};
    xyz next = {180, -40, -250};
    xyz dot1 = {180, -40, -282};
    xyz dot2 = {190, -50, -250};
    xyz dot3 = {190, -50, -282};
    xyz dot4 = {160, -60, -250};
    xyz dot5 = {160, -60, -282};
    xyz dot6 = {170, -70, -250};
    xyz dot7 = {170, -70, -285};
    out << f_dirsteps(curr, next);
    out << f_dirsteps(next, dot1);
    out << f_dirsteps(dot1, next);
    out << f_dirsteps(next, dot2);
    out << f_dirsteps(dot2, dot3);
    out << f_dirsteps(dot3, dot2);
    out << f_dirsteps(dot2, dot4);
    out << f_dirsteps(dot4, dot5);
    out << f_dirsteps(dot5, dot4);
    out << f_dirsteps(dot4, dot6);
    out << f_dirsteps(dot6, dot7);
    out << f_dirsteps(dot7, dot6);
    out << f_dirsteps(dot6, curr);

    /*   xyz next = {130, -20, -250};
       out << f_dirsteps(curr, next);
       curr = next;
       next = {130, -20, -282};
       out << f_dirsteps(curr, next);
       curr = next;
       for (int i = 0; i < 150; i++) {
           next.x = 180 + 50 * cos(an);
           next.y = -20 + 50 * sin(an);
           next.z = -282;
           out << f_dirsteps(curr, next);
           an -= 0.05;
           curr = next;
       }
     */
    /*    for (int j = 200; j > 0; j -= 3) {
            next.x = 100;
            next.y = -j;
            next.z = -296;
            out << f_dirsteps(curr, next);
            curr = next;
        }
     */
    // next = {200, 0, -250};
    // out << f_dirsteps(curr, next);

    /*
    //  xyz dot1 = {100, 100, -260};
    xyz dot2 = {200, 100, -260};
    xyz dot3 = {200, 0, -260};

    out << f_dirsteps(curr, next);
    out << f_dirsteps(next, dot2);
    // out << f_dirsteps(dot1, dot2);
    out << f_dirsteps(dot2, dot3);
    out << f_dirsteps(dot3, curr);
    */
    out.close();

    return 0;
}

angles cal_angle(xyz coord) {
    float p = 0.0;
    angles angle;
    p = sqrt(coord.x * coord.x + coord.y * coord.y + coord.z * coord.z);
    if (coord.x > 0)
        angle.a = asin(coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) *
                  180 / PI;
    else
        angle.a = asin(-coord.y / sqrt(coord.x * coord.x + coord.y * coord.y)) *
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