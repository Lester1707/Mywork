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
    Vector3 operator + (const Vector3 &v) {
        return {x + v.x, y + v.y, z + v.z};
    }
    Vector3 operator - (const Vector3 &v) {
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

class Piramide {
public:
    Vector3 center;
    Vector3 zenit;
    std::vector <Vector3> down;
    float edge;
    Piramide() : down(4), flats(5) {
        edge = 1.f;
        center = {0, 0, 0};
        Norm();
    }
    Piramide(const Vector3 &v, const float &e) : down(4), flats(5) {
        edge = e;
        center = v;
        Norm();
    }
     std::vector <std::vector<Vector3>>& getFlats() {
        for (size_t i = 0; i < 4; i++) {
            flats[i] = {zenit, down[i], i + 1 == 4 ? down[0] : down[i + 1]};
        }
        flats[flats.size() - 1] = {down[0], down[1], down[2], down[3]};
        return flats;
    }
    void Norm() {
        down[0] = {center.x + edge/2, center.y + edge/2, center.z - edge/(2.f*(float)sqrt(2))};
        down[1] = {center.x - edge/2, center.y + edge/2, center.z - edge/(2.f*(float)sqrt(2))};
        down[2] = {center.x - edge/2, center.y - edge/2, center.z - edge/(2.f*(float)sqrt(2))};
        down[3] = {center.x + edge/2, center.y - edge/2, center.z - edge/(2.f*(float)sqrt(2))};
        zenit = center + Vector3(0, 0, edge/(2*sqrt(2)));
    }
    void y_rotate() {
        float df = 0.1f;
        QGenericMatrix<1, 3, float> input;
        QGenericMatrix<1, 3, float> output;
        float rotate[] = {
            (float)cos(df), 0, (float)sin(df),
            0, 1, 0,
            -(float)sin(df), 0, (float)cos(df)
        };
        float points[3];
        QMatrix3x3 m(rotate);
        for (size_t i = 0; i < down.size(); i++) {
            points[0] = down[i].x;
            points[1] = down[i].y;
            points[2] = down[i].z;
            input = QGenericMatrix<1, 3, float>(points);
            output = m*input;
            down[i].x = *output.data();
            down[i].y = *(output.data() + 1);
            down[i].z = *(output.data() + 2);
        }
        points[0] = zenit.x;
        points[1] = zenit.y;
        points[2] = zenit.z;
        input = QGenericMatrix<1, 3, float>(points);
        output = m*input;
        zenit.x = *output.data();
        zenit.y = *(output.data() + 1);
        zenit.z = *(output.data() + 2);
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
        for (size_t i = 0; i < down.size(); i++) {
            points[0] = down[i].x;
            points[1] = down[i].y;
            points[2] = down[i].z;
            input = QGenericMatrix<1, 3, float>(points);
            output = m*input;
            down[i].x = *output.data();
            down[i].y = *(output.data() + 1);
            down[i].z = *(output.data() + 2);
        }
        points[0] = zenit.x;
        points[1] = zenit.y;
        points[2] = zenit.z;
        input = QGenericMatrix<1, 3, float>(points);
        output = m*input;
        zenit.x = *output.data();
        zenit.y = *(output.data() + 1);
        zenit.z = *(output.data() + 2);
    }
private:
    std::vector <std::vector<Vector3>> flats;
};

class Camera {
private:
    std::vector<std::pair<Vector3, Vector3>> edges;
    enum SPACE {
      S_LEFT, S_RIGHT, S_UP, S_DOWN,
      S_FORWARD, S_BACK, S_LEFT_SP, S_LEFT_F,
      S_UP_F, S_RIGHT_F, S_DOWN_F
    };
    float pointDistance(const Vector3 &p1, const Vector3 &p2) {
        return sqrt(pow_2(p2.x-p1.x) + pow_2(p2.y-p1.y) + pow_2(p2.z-p1.z));
    }
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
    Camera() {
        ang_vision_down = M_PI/2;
        ang_vision_up = M_PI/2;
        position = {0, -2, 0};
        rotate_y = 0.f;
        izo_mode = false;
        izo_edge = 0.f;
    }
    std::vector<std::pair<Vector3, Vector3>>& getFlat(Vector3 &center, const std::vector <Vector3> &points) {
        edges.clear();
        if (points.size() < 3)
            return edges;
        Vector3 n = Vector3(
                    det_2(points[1].y - points[0].y, points[1].z - points[0].z, points[2].y - points[0].y, points[2].z - points[0].z),
                    -det_2(points[1].x - points[0].x, points[1].z - points[0].z, points[2].x - points[0].x, points[2].z - points[0].z),
                    det_2(points[1].x - points[0].x, points[1].y - points[0].y, points[2].x - points[0].x, points[2].y - points[0].y));
        if (pointDistance(n, Vector3(0, 0, 0)) < 0.000001f)
            return edges;
        if (scalar(center - points[0], n) < 0)
            n = n*-1.f;
        if (izo_mode) {
            if (scalar(getNormalVector(S_FORWARD), n) < 0)
                return edges;
        } else
            if (scalar(position - points[0], n) > 0)
                return edges;
        std::pair<Vector3, Vector3> p;
        for (size_t i = 0; i < points.size(); i++) {
            p = getLine(points[i], i + 1 == points.size() ? points[0] : points[i + 1]);
            edges.push_back(p);
        }
        return edges;
    }
    std::pair<Vector3, Vector3> getLine(const Vector3 &point1, const Vector3 &point2) {
        if (izo_mode) {
            Vector3 p1 = point1;
            Vector3 p2 = point2;
            if (pointDistance(position, point1) >= pointDistance(position, point2))
                p2 = sharePoint(S_FORWARD, p1, p2, getNormalVector(S_FORWARD));
            else
                p1 = sharePoint(S_FORWARD, p2, p1, getNormalVector(S_FORWARD));
            float l1 = distanceToSpace(getNormalVector(S_LEFT),
                Vector3(position.x + izo_edge/2*getNormalVector(S_RIGHT).x, position.y + izo_edge/2*getNormalVector(S_RIGHT).y, 0), p1);
            float u1 = distanceToSpace(getNormalVector(S_UP), position + Vector3(0, 0, izo_edge/2), p1);
            float l2 = distanceToSpace(getNormalVector(S_LEFT),
                Vector3(position.x + izo_edge/2*getNormalVector(S_RIGHT).x, position.y + izo_edge/2*getNormalVector(S_RIGHT).y, 0), p2);
            float u2 = distanceToSpace(getNormalVector(S_UP), position + Vector3(0, 0, izo_edge/2), p2);
            return {{l1/izo_edge, u1/izo_edge, 0}, {l2/izo_edge, u2/izo_edge, 0}};
        }
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
        return {{ang_l1/ang_vision_down, ang_u1/ang_vision_up, 0}, {ang_l2/ang_vision_down, ang_u2/ang_vision_up, 0}};
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
    Piramide fig;
    Camera cam;
    QPen pen_graph;
    float x0, y0, size;
    World(QWidget *par = 0) : QWidget(par), pen_graph(Qt::black, 1, Qt::SolidLine) {
        size = 700.f;
    }
    void drawFunction(QPainter *qp) {
        fl = fig.getFlats();
        for (size_t i = 0; i < fl.size(); i++) {
            points = cam.getFlat(fig.center, fl[i]);
            for (size_t j = 0; j < points.size(); j++)
                qp->drawLine(QLineF(x0 + 2.f*std::min(x0, y0)*(points[j].first.x - 0.5f), y0 + 2.f*std::min(x0, y0)*(points[j].first.y - 0.5f),
                    x0 + 2.f*std::min(x0, y0)*(points[j].second.x - 0.5f), y0 + 2.f*std::min(x0, y0)*(points[j].second.y - 0.5f)));
        }
    }
    void paintEvent(QPaintEvent *e) {
        Q_UNUSED(e);
        x0 = width()/2.f;
        y0 = height()/2.f;
        QPainter qp(this);
        qp.setRenderHint(QPainter::Antialiasing);
        qp.setPen(pen_graph);
        drawFunction(&qp);
    }
    void keyPressEvent(QKeyEvent *event) {
        int key = event->key();
        if (key == Qt::Key_Left) {
            float df = 0.1f;
            float R = pow_2(cam.position.x) + pow_2(cam.position.y);
            float l = sqrt(2.f*R*(1.f - cos(df)));
            cam.position.x -= l*cos(cam.rotate_y - df/2);
            cam.position.y -= l*sin(cam.rotate_y - df/2);
            cam.rotate_y -= df;
        }
        if (key == Qt::Key_Right) {
            float df = 0.1f;
            float R = pow_2(cam.position.x) + pow_2(cam.position.y);
            float l = sqrt(2.f*R*(1.f - cos(df)));
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
