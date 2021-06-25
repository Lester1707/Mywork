#include <vector>
#include <cmath>
#include <QWidget>
#include <iostream>
#include <algorithm>
#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMatrix3x3>
#include <QVector3D>
#include <algorithm>
#ifndef WORLD_H
#define WORLD_H


float pow_2(const float &a) {
    return a*a;
}

float det_2(const float &a11, const float &a12, const float &a21, const float &a22) {
    return a11*a22 - a12*a21;
}

struct Vector3 {
    float x;
    float y;
    float z;
    bool broken;
    Vector3() {
        x = 0;
        y = 0;
        z = 0;
        broken = true;
    }
    Vector3(const Vector3 &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        broken = v.broken;
    }
    Vector3(const float &x_cord, const float &y_cord, const float &z_cord) {
        x = x_cord;
        y = y_cord;
        z = z_cord;
        broken = false;
    }
    Vector3 operator + (const Vector3 &v) const {
        return {x + v.x, y + v.y, z + v.z};
    }
    Vector3 operator - (const Vector3 &v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    Vector3 operator / (const float &a) {
        if (a == 0)
            return Vector3();
        return {x/a, y/a, z/a};
    }
    Vector3 operator * (const float &a) {
        return {x*a, y*a, z*a};
    }
    friend float scalar(const Vector3 &v1, const Vector3 &v2) {
        return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z)/(sqrt(pow_2(v1.x) + pow_2(v1.y) + pow_2(v1.z))*sqrt(pow_2(v2.x) + pow_2(v2.y) + pow_2(v2.z)));
    }
    void operator = (const Vector3 &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        broken = v.broken;
    }
};

Vector3 Normal(const Vector3 &points0, const Vector3 &points1, const Vector3 &points2) {
    return Vector3(
        det_2(points1.y - points0.y, points1.z - points0.z, points2.y - points0.y, points2.z - points0.z),
        -det_2(points1.x - points0.x, points1.z - points0.z, points2.x - points0.x, points2.z - points0.z),
        det_2(points1.x - points0.x, points1.y - points0.y, points2.x - points0.x, points2.y - points0.y));
}

class Cylinder {
public:
    Vector3 center;
    Vector3 zenit;
    Vector3 nadir;
    std::vector <std::vector <Vector3>> down, up, bok;
    float a;
    float b;
    float h;
    float h_shift;
    float dl;
    int reflect;
    Cylinder(const float &r1, const float &r2, const float &h_d, const float &h_s) {
        a = r1;
        b = r2;
        center = {0.f, 0.f, 0.f};
        h = h_d;
        h_shift = h_s;
        df_r = 0.01f;
        reflect = 11;
        Norm();
    }
    std::vector <std::vector<Vector3>>& calculateFlats() {
        flats.clear();
        for (size_t i = 0; i + 1 < down[0].size(); i++)
            flats.push_back({nadir, down[0][i], down[0][i + 1]});
        flats.push_back({nadir, down[0][down[0].size() - 1], down[0][0]});
        for (size_t i = 0; i + 1 < down.size(); i++) {
            for (size_t j = 0; j + 1 < down[i].size(); j++)
                flats.push_back({down[i][j], down[i + 1][j], down[i + 1][j + 1], down[i][j + 1]});
            flats.push_back({down[i][down[i].size() - 1], down[i + 1][down[i].size() - 1], down[i + 1][0], down[i][0]});
        }
        for (size_t i = 0; i + 1 < up[0].size(); i++)
            flats.push_back({zenit, up[0][i], up[0][i + 1]});
        flats.push_back({zenit, up[0][up[0].size() - 1], up[0][0]});
        for (size_t i = 0; i + 1 < up.size(); i++) {
            for (size_t j = 0; j + 1 < up[i].size(); j++)
                flats.push_back({up[i][j], up[i + 1][j], up[i + 1][j + 1], up[i][j + 1]});
            flats.push_back({up[i][up[i].size() - 1], up[i + 1][up[i].size() - 1], up[i + 1][0], up[i][0]});
        }
        flats.push_back({bok[0][0],  bok[1][0], bok[0][0].z <= bok[1][0].z ? bok[1][1] : bok[0][1]});
        flats.push_back({bok[0][0], bok[0][1], bok[0][0].z <= bok[1][0].z ? bok[1][1] : bok[bok.size() - 1][1]});
        for (size_t i = 1; i + 1 < bok.size(); i++) {
            if (bok[i][0].z <= bok[i + 1][0].z) {
                flats.push_back({bok[i][0],  bok[i + 1][0], bok[i + 1][1]});
                flats.push_back({bok[i][0], bok[i][1],bok[i + 1][1]});
            } else {
                flats.push_back({bok[i][0],  bok[i + 1][0], bok[i][1]});
                flats.push_back({bok[i][0], bok[i][1], bok[i - 1][1]});
            }
        }
        flats.push_back({bok[bok.size() - 1][0],  bok[0][0], bok[bok.size() - 1][0].z <= bok[0][0].z ? bok[0][1] : bok[bok.size() - 1][1]});
        flats.push_back({bok[bok.size() - 1][0], bok[bok.size() - 1][1], bok[bok.size() - 1][0].z <= bok[0][0].z ? bok[0][1] : bok[bok.size() - 2][1]});
        for (size_t i = 0; i + 1 < bok.size(); i++)
            for (size_t j = 1; j + 1 < bok[i].size(); j++)
                    flats.push_back({bok[i][j], bok[i + 1][j], bok[i + 1][j + 1], bok[i][j + 1]});
        for (size_t j = 1; j + 1 < bok[bok.size() - 1].size(); j++)
                flats.push_back({bok[bok.size() - 1][j], bok[0][j], bok[0][j + 1], bok[bok.size() - 1][j + 1]});
        return flats;
    }
    std::vector <std::vector<Vector3>>& getFlats() {
       size_t q = 0;
       for (size_t i = 0; i + 1 < down[0].size(); i++) {
           flats[q][0] = nadir;
           flats[q][1] = down[0][i];
           flats[q][2] = down[0][i + 1];
           q++;
       }
       flats[q][0] = nadir;
       flats[q][1] = down[0][down[0].size() - 1];
       flats[q][2]= down[0][0];
       q++;
       for (size_t i = 0; i + 1 < down.size(); i++) {
           for (size_t j = 0; j + 1 < down[i].size(); j++) {
               flats[q][0] = down[i][j];
               flats[q][1] = down[i + 1][j];
               flats[q][2] = down[i + 1][j + 1];
               flats[q][3] = down[i][j + 1];
               q++;
           }
           flats[q][0] = down[i][down[i].size() - 1];
           flats[q][1] = down[i + 1][down[i].size() - 1];
           flats[q][2] = down[i + 1][0];
           flats[q][3] = down[i][0];
           q++;
       }
       for (size_t i = 0; i + 1 < up[0].size(); i++) {
           flats[q][0] = zenit;
           flats[q][1] = up[0][i];
           flats[q][2] = up[0][i + 1];
           q++;
       }
      flats[q][0] = zenit;
      flats[q][1] = up[0][up[0].size() - 1];
      flats[q][2] = up[0][0];
      q++;
       for (size_t i = 0; i + 1 < up.size(); i++) {
           for (size_t j = 0; j + 1 < up[i].size(); j++) {
               flats[q][0] = up[i][j];
               flats[q][1] = up[i + 1][j];
               flats[q][2] = up[i + 1][j + 1];
               flats[q][3] = up[i][j + 1];
               q++;
           }
           flats[q][0] = up[i][up[i].size() - 1];
           flats[q][1] = up[i + 1][up[i].size() - 1];
           flats[q][2] = up[i + 1][0];
           flats[q][3] = up[i][0];
           q++;
       }
       flats[q][0] = bok[0][0];
       flats[q][1] = bok[1][0];
       flats[q][2] = bok[0][0].z <= bok[1][0].z ? bok[1][1] : bok[0][1];
       q++;
       flats[q][0] = bok[0][0];
       flats[q][1] = bok[0][1];
       flats[q][2] = bok[0][0].z <= bok[1][0].z ? bok[1][1] : bok[bok.size() - 1][1];
       q++;
       for (size_t i = 1; i + 1 < bok.size(); i++) {
           if (bok[i][0].z <= bok[i + 1][0].z) {
               flats[q][0] = bok[i][0];
               flats[q][1] = bok[i + 1][0];
               flats[q][2] = bok[i + 1][1];
               q++;
               flats[q][0] = bok[i][0];
               flats[q][1] = bok[i][1];
               flats[q][2] = bok[i + 1][1];
               q++;
           } else {
               flats[q][0] = bok[i][0];
               flats[q][1] = bok[i + 1][0];
               flats[q][2] = bok[i][1];
               q++;
               flats[q][0] = bok[i][0];
               flats[q][1] = bok[i][1];
               flats[q][2] = bok[i - 1][1];
               q++;
           }
       }
       flats[q][0] = bok[bok.size() - 1][0];
       flats[q][1] = bok[0][0];
       flats[q][2] = bok[bok.size() - 1][0].z <= bok[0][0].z ? bok[0][1] : bok[bok.size() - 1][1];
       q++;
       flats[q][0] = bok[bok.size() - 1][0];
       flats[q][1] = bok[bok.size() - 1][1];
       flats[q][2] = bok[bok.size() - 1][0].z <= bok[0][0].z ? bok[0][1] : bok[bok.size() - 2][1];
       q++;
       for (size_t i = 0; i + 1 < bok.size(); i++)
           for (size_t j = 1; j + 1 < bok[i].size(); j++) {
               flats[q][0] = bok[i][j];
               flats[q][1] = bok[i + 1][j];
               flats[q][2] = bok[i + 1][j + 1];
               flats[q][3] = bok[i][j + 1];
               q++;
           }
       for (size_t j = 1; j + 1 < bok[bok.size() - 1].size(); j++) {
               flats[q][0] = bok[bok.size() - 1][j];
               flats[q][1] = bok[0][j];
               flats[q][2] = bok[0][j + 1];
               flats[q][3] = bok[bok.size() - 1][j + 1];
               q++;
       }
       return flats;
   }
    void Norm() {
        down.clear();
        bok.clear();
        up.clear();
        float inclin_t = h_shift/2/a;
        Vector3 center_new = Vector3(center.x, center.y, center.z + (h - h_shift)/2 + a*inclin_t);
        float l;
        float da = 0.01f;
        float C = (b/a);
        float step = 2.f*M_PI/50;
        std::vector <Vector3> tmp;
        for (float r = da; r <= a; r += da) {
            for (float f = 0.f; f <= 2*M_PI; f += step) {
                l = C*r/sqrt(pow_2(sin(f)) + pow_2(C*cos(f)));
                tmp.push_back(Vector3(center.x + l*cos(f), center.y + l*sin(f), center.z - (h + h_shift)/2));
            }
            down.push_back(tmp);
            tmp.clear();
        }
        up = down;
        for (size_t i = 0; i < up.size(); i++) {
            for (size_t j = 0; j < up[i].size(); j++)
                up[i][j] = up[i][j] + Vector3(0, 0, (up[i][j].x + a)*inclin_t + h);
        }
        for (size_t i = 0; i < up[up.size() - 1].size(); i++) {
            tmp.push_back(up[up.size() - 1][i]);
            tmp.push_back(Vector3(tmp[0].x, tmp[0].y, up[up.size() - 1][i != 0 ? i - 1 : up[up.size() - 1].size() - 1].z <= tmp[0].z ?
                        up[up.size() - 1][i != 0 ? i - 1 : up[up.size() - 1].size() - 1].z : up[up.size() - 1][i + 1 == up[up.size() - 1].size() ?
                        0 : i + 1].z));
            bok.push_back(tmp);
            tmp.clear();
        }
        for (size_t i = 0; i < bok.size(); i++) {
            size_t scale = 100.f;
            float d_step = (bok[i][1].z + (h + h_shift)/2.f)/scale;
            for (size_t j = 0; j < scale; j++) {
                bok[i].push_back(bok[i][j + 1] - Vector3(0, 0, d_step));
            }
        }
        zenit = center_new;
        nadir = center - Vector3(0.f, 0.f, (h + h_shift)/2);
        calculateFlats();
    }
    void y_rotate() {
        QGenericMatrix<1, 3, float> input;
        QGenericMatrix<1, 3, float> output;
        float rotate_y[] = {
            (float)cos(df_r), 0, (float)sin(df_r),
            0, 1, 0,
            -(float)sin(df_r), 0, (float)cos(df_r)
        };
        float points[3];
        QMatrix3x3 m_y(rotate_y);
        for (size_t i = 0; i < down.size(); i++)
            for (size_t j = 0; j < down[i].size(); j++) {
                points[0] = down[i][j].x;
                points[1] = down[i][j].y;
                points[2] = down[i][j].z;
                input = QGenericMatrix<1, 3, float>(points);
                output = m_y*input;
                down[i][j].x = *output.data();
                down[i][j].y = *(output.data() + 1);
                down[i][j].z = *(output.data() + 2);
            }
        for (size_t i = 0; i < up.size(); i++)
            for (size_t j = 0; j < up[i].size(); j++) {
                points[0] = up[i][j].x;
                points[1] = up[i][j].y;
                points[2] = up[i][j].z;
                input = QGenericMatrix<1, 3, float>(points);
                output = m_y*input;
                up[i][j].x = *output.data();
                up[i][j].y = *(output.data() + 1);
                up[i][j].z = *(output.data() + 2);
            }
        for (size_t i = 0; i < bok.size(); i++)
            for (size_t j = 0; j < bok[i].size(); j++) {
                points[0] = bok[i][j].x;
                points[1] = bok[i][j].y;
                points[2] = bok[i][j].z;
                input = QGenericMatrix<1, 3, float>(points);
                output = m_y*input;
                bok[i][j].x = *output.data();
                bok[i][j].y = *(output.data() + 1);
                bok[i][j].z = *(output.data() + 2);
            }
        points[0] = zenit.x;
        points[1] = zenit.y;
        points[2] = zenit.z;
        input = QGenericMatrix<1, 3, float>(points);
        output = m_y*input;
        zenit.x = *output.data();
        zenit.y = *(output.data() + 1);
        zenit.z = *(output.data() + 2);
        points[0] = nadir.x;
        points[1] = nadir.y;
        points[2] = nadir.z;
        input = QGenericMatrix<1, 3, float>(points);
        output = m_y*input;
        nadir.x = *output.data();
        nadir.y = *(output.data() + 1);
        nadir.z = *(output.data() + 2);
    }
    void x_rotate() {
        float df = 0.1f;
        QGenericMatrix<1, 3, float> input;
        QGenericMatrix<1, 3, float> output;
        float rotate[] = {
            1, 0, 0,
            0, (float)cos(df), -(float)sin(df),
            0, (float)sin(df), (float)cos(df)
        };
        float points[3];
        QMatrix3x3 m(rotate);
        for (size_t i = 0; i < down.size(); i++)
            for (size_t j = 0; j < down[i].size(); j++) {
                points[0] = down[i][j].x;
                points[1] = down[i][j].y;
                points[2] = down[i][j].z;
                input = QGenericMatrix<1, 3, float>(points);
                output = m*input;
                down[i][j].x = *output.data();
                down[i][j].y = *(output.data() + 1);
                down[i][j].z = *(output.data() + 2);
            }
        for (size_t i = 0; i < up.size(); i++)
            for (size_t j = 0; j < up[i].size(); j++) {
                points[0] = up[i][j].x;
                points[1] = up[i][j].y;
                points[2] = up[i][j].z;
                input = QGenericMatrix<1, 3, float>(points);
                output = m*input;
                up[i][j].x = *output.data();
                up[i][j].y = *(output.data() + 1);
                up[i][j].z = *(output.data() + 2);
            }
        for (size_t i = 0; i < bok.size(); i++)
            for (size_t j = 0; j < bok[i].size(); j++) {
                points[0] = bok[i][j].x;
                points[1] = bok[i][j].y;
                points[2] = bok[i][j].z;
                input = QGenericMatrix<1, 3, float>(points);
                output = m*input;
                bok[i][j].x = *output.data();
                bok[i][j].y = *(output.data() + 1);
                bok[i][j].z = *(output.data() + 2);
            }
        points[0] = zenit.x;
        points[1] = zenit.y;
        points[2] = zenit.z;
        input = QGenericMatrix<1, 3, float>(points);
        output = m*input;
        zenit.x = *output.data();
        zenit.y = *(output.data() + 1);
        zenit.z = *(output.data() + 2);
        points[0] = nadir.x;
        points[1] = nadir.y;
        points[2] = nadir.z;
        input = QGenericMatrix<1, 3, float>(points);
        output = m*input;
        nadir.x = *output.data();
        nadir.y = *(output.data() + 1);
        nadir.z = *(output.data() + 2);
    }
private:
    std::vector <std::vector<Vector3>> flats;
    float df_r;
};

class Camera {
private:
    std::vector<std::pair<Vector3, Vector3>> edges;
    enum SPACE {
      S_LEFT, S_RIGHT, S_UP, S_DOWN,
      S_FORWARD, S_BACK, S_LEFT_SP, S_LEFT_F,
      S_UP_F, S_RIGHT_F, S_DOWN_F
    };
    float Dfactor(const Vector3 &n, const Vector3 &p) {
        return -(n.x*p.x + n.y*p.y + n.z*p.z);
    }
    Vector3 sharePoint(const SPACE &type, const Vector3 &s_p, const Vector3 &point, const Vector3 &v) {
        Vector3 n = getNormalVector(type);
        if (fabs(n.x*v.x + n.y*v.y + n.z*v.z) < 0.000001f)
            return Vector3();
        float t;
        t = -(n.x*point.x + n.y*point.y + n.z*point.z + Dfactor(n, s_p))/(n.x*v.x + n.y*v.y + n.z*v.z);
        return {v.x*t + point.x, v.y*t + point.y, v.z*t + point.z};
    }
    float distanceToSpace(const Vector3 &n, const Vector3 &p_s, const Vector3 &p) {
        return (n.x*p.x + n.y*p.y + n.z*p.z + Dfactor(n, p_s))/sqrt(pow_2(n.x) + pow_2(n.y) + pow_2(n.z));
    }
    Vector3 getNormalVector(const SPACE &type) {
        if (izo_mode)
            switch(type) {
                case S_LEFT:
                    return {-(float)position.y/(float)sqrt(pow_2(position.x) + pow_2(position.y)), (float)position.x/(float)sqrt(pow_2(position.x) + pow_2(position.y)), 0};
                case S_RIGHT:
                    return {(float)position.y/(float)sqrt(pow_2(position.x) + pow_2(position.y)), -(float)position.x/(float)sqrt(pow_2(position.x) + pow_2(position.y)), 0};
                case S_UP:
                    return {0, 0, -1};
                case S_FORWARD:
                    return {-(float)position.x/(float)sqrt(pow_2(position.x) + pow_2(position.y)), -(float)position.y/(float)sqrt(pow_2(position.x) + pow_2(position.y)), 0};
                default:
                    break;
            }
        else
            switch(type) {
                case S_LEFT:
                    return {(float)cos(rotate_y + ang_vision_down/2), (float)sin(rotate_y + ang_vision_down/2), 0};
                case S_RIGHT:
                    return {(float)-cos(rotate_y + ang_vision_down/2), (float)sin(rotate_y + ang_vision_down/2), 0};
                case S_UP:
                    return {(float)sin(rotate_y)*(float)sin(ang_vision_up/2), (float)-cos(rotate_y)*(float)sin(ang_vision_up/2), (float)cos(ang_vision_up/2)};
                case S_FORWARD:
                    return {(float)-sin(rotate_y), (float)cos(rotate_y), 0};
                case S_LEFT_F:
                    return {(float)-sin(rotate_y + ang_vision_down/2), (float)cos(rotate_y + ang_vision_down/2), 0};
                case S_UP_F:
                    return {(float)-sin(rotate_y)*(float)cos(ang_vision_up/2), (float)cos(rotate_y)*(float)cos(ang_vision_up/2), (float)sin(ang_vision_up/2)};
                case S_LEFT_SP:
                    return {(float)cos(rotate_y), (float)sin(rotate_y), 0};
                default:
                    break;
            }
        return Vector3();
    }
public:
    float rotate_y;
    Vector3 position;
    float ang_vision_down;
    float ang_vision_up;
    bool izo_mode;
    float izo_edge;
    Camera() : edges(4) {
        ang_vision_down = M_PI/2;
        ang_vision_up = M_PI/2;
        position = {0, -2, 0};
        rotate_y = 0.f;
        izo_mode = false;
        izo_edge = 0.f;
    }
    float pointDistance(const Vector3 &p1, const Vector3 &p2) {
        return sqrt(pow_2(p2.x-p1.x) + pow_2(p2.y-p1.y) + pow_2(p2.z-p1.z));
    }
    std::vector<std::pair<Vector3, Vector3>>& getFlat(Vector3 &center, const std::vector <Vector3> &points) {
        edges[3] = std::pair<Vector3, Vector3>(Vector3(), Vector3());
//        if (points.size() < 3)
//            return edges;
        Vector3 n = Normal(points[0], points[1], points[2]);
        if (pointDistance(n, Vector3(0, 0, 0)) < 0.000001f) {
            edges[0] = std::pair<Vector3, Vector3>(Vector3(), Vector3());
            return edges;
        }
        if (scalar(center - points[0], n) < 0)
            n = n*-1.f;
        if (scalar(position - points[0], n) > 0) {
            edges[0] = std::pair<Vector3, Vector3>(Vector3(), Vector3());
            return edges;
        }
        std::pair<Vector3, Vector3> p;
        for (size_t i = 0; i + 1 < points.size(); i++) {
            p = getLine(points[i], points[i + 1]);
            edges[i] = p;
        }
        p = getLine(points[points.size() - 1], points[0]);
        edges[points.size() - 1] = p;
        return edges;
    }
    std::pair<Vector3, Vector3> getLine(const Vector3 &point1, const Vector3 &point2) {
        float ang_l1 = acos(scalar(Vector3(point1.x - position.x, point1.y - position.y, 0), getNormalVector(S_LEFT_F)));
        if (scalar(Vector3(point1.x - position.x, point1.y - position.y, 0), getNormalVector(S_FORWARD)) < cos(ang_vision_down/2) && ang_l1 < ang_vision_down)
            ang_l1 *= -1;
        Vector3 up_p1 = sharePoint(S_LEFT_SP, position, point1, getNormalVector(S_LEFT_SP));
        float ang_u1 = acos(scalar(up_p1 - position, getNormalVector(S_UP_F)));
        if (scalar(up_p1 - position, getNormalVector(S_FORWARD)) < cos(ang_vision_up/2) && ang_u1 < ang_vision_up)
            ang_u1 *= -1;
        float ang_l2 = acos(scalar(Vector3(point2.x - position.x, point2.y - position.y, 0), getNormalVector(S_LEFT_F)));
        if (scalar(Vector3(point2.x - position.x, point2.y - position.y, 0), getNormalVector(S_FORWARD)) < cos(ang_vision_down/2) && ang_l2 < ang_vision_down)
            ang_l2 *= -1;
        Vector3 up_p2 = sharePoint(S_LEFT_SP, position, point2, getNormalVector(S_LEFT_SP));
        float ang_u2 = acos(scalar(up_p2 - position, getNormalVector(S_UP_F)));
        if (scalar(up_p2 - position, getNormalVector(S_FORWARD)) < cos(ang_vision_up/2) && ang_u2 < ang_vision_up)
            ang_u2 *= -1;
        return std::pair<Vector3, Vector3>(Vector3(ang_l1/ang_vision_down, ang_u1/ang_vision_up, 0), Vector3(ang_l2/ang_vision_down, ang_u2/ang_vision_up, 0));
    }
    Vector3 getPoint(const Vector3 &point) {
        float ang_l1 = acos(scalar(Vector3(point.x - position.x, point.y - position.y, 0), getNormalVector(S_LEFT_F)));
        if (scalar(Vector3(point.x - position.x, point.y - position.y, 0), getNormalVector(S_FORWARD)) < cos(ang_vision_down/2) && ang_l1 < ang_vision_down)
            ang_l1 *= -1;
        Vector3 up_p1 = sharePoint(S_LEFT_SP, position, point, getNormalVector(S_LEFT_SP));
        float ang_u1 = acos(scalar(up_p1 - position, getNormalVector(S_UP_F)));
        if (scalar(up_p1 - position, getNormalVector(S_FORWARD)) < cos(ang_vision_up/2) && ang_u1 < ang_vision_up)
            ang_u1 *= -1;
        return Vector3(ang_l1/ang_vision_down, ang_u1/ang_vision_up, 0);
    }
    void changeMode(const float &sz) {
        if (izo_mode) {
            izo_edge = 0.f;
            izo_mode = false;
        } else {
            izo_mode = true;
            izo_edge = sz;
        }
    }
};

class World : public QWidget {
public:
    Cylinder fig;
    Camera cam;
    float x0, y0, size;
    Vector3 light;
    float intensivity;
    World(QWidget *par = 0) : QWidget(par), fig(0.2f, 0.3f, 0.3f, 0.4f) {
        size = 700.f;
        light = {0.f, -2.f, 0.f};
        intensivity = 20.f*M_PI_2;
        setStyleSheet("background-color:black");
    }
    void drawFunction(QPainter *qp) {
        fl = fig.getFlats();
        Vector3 l = cam.getPoint(light);
        int color = 255*(2.f/M_PI*atan(intensivity/pow_2(cam.pointDistance(light, cam.position))));
        qp->setBrush(QBrush(QColor(color, color, color)));
        qp->drawEllipse(QPointF(width()*l.x, height()*l.y), 5.f, 5.f);
        Vector3 n;
        for (size_t i = 0; i < fl.size(); i++) {
            points = cam.getFlat(fig.center, fl[i]);
            QPolygon space;
            for (size_t j = 0; j < points.size(); j++) {
                if (points[j].first.broken)
                    break;
                space << QPoint((width()*points[j].first.x), (height()*points[j].first.y));
            }
            if (points[0].first.broken)
                continue;
            n = Normal(fl[i][0], fl[i][1], fl[i][2]);
            if (scalar(fig.center - fl[i][1], n) < 0)
                n = n*-1.f;
            float k = 2.f/M_PI*atan(intensivity/pow_2(cam.pointDistance(light, fl[i][0])))*pow(scalar(n, fl[i][0] - light), fig.reflect);
            color = (k < 10.f/255 ? 10 : 255*k);
            qp->setPen(QPen(QColor(color, color, color), 0, Qt::SolidLine));
            qp->setBrush(QColor(color, color, color));
            qp->drawPolygon(space);
        }
    }
    void paintEvent(QPaintEvent *e) {
        Q_UNUSED(e);
        QPainter qp(this);
        drawFunction(&qp);
    }
    void keyPressEvent(QKeyEvent *event) {
        int key = event->key();
        if (key == Qt::Key_Left) {
            float df = 0.1f;
            float R = pow_2(cam.position.x) + pow_2(cam.position.y);
            float l = sqrt(R)*df;
            cam.position.x -= l*cos(cam.rotate_y - df/2);
            cam.position.y -= l*sin(cam.rotate_y - df/2);
            cam.rotate_y -= df;
        }
        if (key == Qt::Key_Right) {
            float df = 0.1f;
            float R = pow_2(cam.position.x) + pow_2(cam.position.y);
            float l = sqrt(R)*df;
            cam.position.x += l*cos(cam.rotate_y + df/2);
            cam.position.y += l*sin(cam.rotate_y + df/2);
            cam.rotate_y += df;
        }
        if (key == Qt::Key_Up) {
            fig.y_rotate();
        }
        if (key == Qt::Key_Down) {
            fig.x_rotate();
        }
        if (key == Qt::Key_Backspace)
            cam.changeMode(3.f);
        if (key == Qt::Key_W)
            intensivity += 2.f;
        if (key == Qt::Key_S) {
            if (intensivity > 0.f)
                intensivity -= 2.f;
        }
        if (key == Qt::Key_A) {
            if (fig.reflect >= 3)
                fig.reflect -= 2;
        }
        if (key == Qt::Key_D)
            fig.reflect += 2;
        repaint();
    }
    void wheelEvent(QWheelEvent *event) {
        float s_a = cam.position.x/sqrt(pow_2(cam.position.x) + pow_2(cam.position.y));
        float c_a = cam.position.y/sqrt(pow_2(cam.position.x) + pow_2(cam.position.y));
        float l = -event->delta()/2000.f;
        cam.position.x += l*s_a;
        cam.position.y += l*c_a;
        repaint();
    }
private:
    std::vector<std::pair<Vector3, Vector3>> points;
    std::vector <std::vector<Vector3>> fl;
};

#endif // WORLD_H
