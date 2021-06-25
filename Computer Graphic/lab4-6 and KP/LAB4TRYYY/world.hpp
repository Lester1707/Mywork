#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>
#ifndef WORLD_H
#define WORLD_H

#define M_PI 3.14159265358979


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
    std::vector <float> flats;
    std::vector <unsigned int> indexes;
    std::vector <float> normals;
	std::vector <unsigned int> borders;
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
        df_r = 0.1f;
        reflect = 11;
        Norm();
    }
    Vector3 normalVector(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3) {
        Vector3 n = Normal(p1, p2, p3);
        if (scalar(p1 - center, n) < 0)
            n = n * -1.f;
        return n;
    }
    void pushBack(std::vector <Vector3> &&v) {
        size_t j = flats.size() / 3;
		borders.push_back(flats.size());
        Vector3 n = normalVector(v[0], v[1], v[2]);
        if (v.size() == 3) {
            indexes.push_back(j);
            indexes.push_back(j + 1);
            indexes.push_back(j + 2);
        }
        else if (v.size() == 4) {
            indexes.push_back(j);
            indexes.push_back(j + 1);
            indexes.push_back(j + 2);
            indexes.push_back(j + 2);
            indexes.push_back(j + 3);
            indexes.push_back(j);
        }
        for (size_t i = 0; i < v.size(); i++) {
            flats.push_back(v[i].x);
            flats.push_back(v[i].y);
            flats.push_back(v[i].z);
            normals.push_back(n.x);
            normals.push_back(n.y);
            normals.push_back(n.z);
        }
    }
    void calculateFlats() {
        flats.clear();
		indexes.clear();
		normals.clear();
        for (size_t i = 0; i + 1 < down[0].size(); i++)
            pushBack(std::vector<Vector3>({ nadir, down[0][i], down[0][i + 1] }));
        pushBack(std::vector<Vector3>({nadir, down[0][down[0].size() - 1], down[0][0]}));
        for (size_t i = 0; i + 1 < down.size(); i++) {
            for (size_t j = 0; j + 1 < down[i].size(); j++)
                pushBack(std::vector<Vector3>({down[i][j], down[i + 1][j], down[i + 1][j + 1], down[i][j + 1]}));
            pushBack(std::vector<Vector3>({down[i][down[i].size() - 1], down[i + 1][down[i].size() - 1], down[i + 1][0], down[i][0]}));
        }
        for (size_t i = 0; i + 1 < up[0].size(); i++)
            pushBack(std::vector<Vector3>({zenit, up[0][i], up[0][i + 1]}));
        pushBack(std::vector<Vector3>({zenit, up[0][up[0].size() - 1], up[0][0]}));
        for (size_t i = 0; i + 1 < up.size(); i++) {
            for (size_t j = 0; j + 1 < up[i].size(); j++)
                pushBack(std::vector<Vector3>({up[i][j], up[i + 1][j], up[i + 1][j + 1], up[i][j + 1]}));
            pushBack(std::vector<Vector3>({up[i][up[i].size() - 1], up[i + 1][up[i].size() - 1], up[i + 1][0], up[i][0]}));
        }
        pushBack(std::vector<Vector3>({bok[0][0],  bok[1][0], bok[0][0].z <= bok[1][0].z ? bok[1][1] : bok[0][1]}));
        pushBack(std::vector<Vector3>({bok[0][0], bok[0][1], bok[0][0].z <= bok[1][0].z ? bok[1][1] : bok[bok.size() - 1][1]}));
        for (size_t i = 1; i + 1 < bok.size(); i++) {
            if (bok[i][0].z <= bok[i + 1][0].z) {
                pushBack(std::vector<Vector3>({bok[i][0],  bok[i + 1][0], bok[i + 1][1]}));
                pushBack(std::vector<Vector3>({bok[i][0], bok[i][1], bok[i + 1][1]}));
            } else {
                pushBack(std::vector<Vector3>({bok[i][0],  bok[i + 1][0], bok[i][1]}));
                pushBack(std::vector<Vector3>({bok[i][0], bok[i][1], bok[i - 1][1]}));
            }
        }
        pushBack(std::vector<Vector3>({bok[bok.size() - 1][0],  bok[0][0], bok[bok.size() - 1][0].z <= bok[0][0].z ? bok[0][1] : bok[bok.size() - 1][1]}));
        pushBack(std::vector<Vector3>({bok[bok.size() - 1][0], bok[bok.size() - 1][1], bok[bok.size() - 1][0].z <= bok[0][0].z ? bok[0][1] : bok[bok.size() - 2][1]}));
        for (size_t i = 0; i + 1 < bok.size(); i++)
            for (size_t j = 1; j + 1 < bok[i].size(); j++)
                pushBack(std::vector<Vector3>({bok[i][j], bok[i + 1][j], bok[i + 1][j + 1], bok[i][j + 1]}));
        for (size_t j = 1; j + 1 < bok[bok.size() - 1].size(); j++)
            pushBack(std::vector<Vector3>({bok[bok.size() - 1][j], bok[0][j], bok[0][j + 1], bok[bok.size() - 1][j + 1]}));
		borders.push_back(flats.size());
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
        float step = 2.f*M_PI/300;
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
            size_t scale = 100;
            float d_step = (bok[i][1].z + (h + h_shift)/2.f)/scale;
            for (size_t j = 0; j < scale; j++) {
                bok[i].push_back(bok[i][j + 1] - Vector3(0, 0, d_step));
            }
        }
        zenit = center_new;
        nadir = center - Vector3(0.f, 0.f, (h + h_shift)/2);
        calculateFlats();
    }
	void changeNormals(const float &time) {
		for (size_t i = 0; i + 1 < borders.size(); i++) {
			Vector3 n = normalVector(Vector3(flats[borders[i]], flats[borders[i] + 1]*cos(time + flats[borders[i] + 1]), flats[borders[i] + 2]),
				Vector3(flats[borders[i] + 3] , flats[borders[i] + 4]* cos(time + flats[borders[i] + 4]), flats[borders[i] + 5]),
				Vector3(flats[borders[i] + 6], flats[borders[i] + 7]* cos(time + flats[borders[i] + 7]), flats[borders[i] + 8]));
			for (size_t j = borders[i]; j < borders[i + 1]; j += 3) {
				normals[j] = n.x;
				normals[j + 1] = n.y;
				normals[j + 2] = n.z;
			}
		}
	}
private:
    float df_r;
};

#endif // WORLD_H
