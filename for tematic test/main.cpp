#include <iostream>
#include <cmath>
#include <vector>

const uint32_t last_root = 5;

float f_d(const float &x) {
    float sum = 0;
    for (uint32_t i = 1; i <= last_root; i++)
        sum += 1.0/(x - float(i));
    return sum;
}

float f(const float &x) {
    float sum = 0;
    for (uint32_t i = 1; i <= last_root; i++)
        sum += log(fabs(x - float(i)));
    return sum;
}

float find(float left, float right) {
    float prev = f_d((left + right)/2.0);
    if (prev >= 0.0)
        left = (left + right)/2.0;
    else if (prev < 0.0)
        right = (left + right)/2.0;
    float now;
    do {
        now = f_d((left + right)/2.0);
        if (now >= 0.0)
            left = (left + right)/2.0;
        else if (now < 0.0)
            right = (left + right)/2.0;
        if (fabs(prev - now) <= __FLT_EPSILON__)
            break;
        prev = now;
    } while (true);
    return (left + right)/2.0;
}

int main() {
    float c;
    std::cout << "Enter the front polinom coefficient: ";
    std::cin >> c;
    if (fabs(c) < __FLT_EPSILON__) {
        std::cout << "To small coefficient" << std::endl;
        throw;
    }
    std::vector <float> points(last_root - 1);
    for (uint32_t i = 1; i < last_root; i++)
        points[i - 1] = find(float(i), float(i + 1));
    float max = 0.0;
    float max_point;
    float min_point;
    float num;
    for (uint16_t i = 0; i < last_root - 1; i += 2) {
        num = f(points[i]);
        if (num > max) {
            max = num;
            max_point = points[i];
        }
    }
    max = 0.0;
    for (uint16_t i = 1; i < last_root - 1; i += 2) {
        num = f(points[i]);
        if (num > max) {
            max = num;
            min_point = points[i];
        }
    }
    if (c > 0.0)
        std::cout << "max point: " << max_point << std::endl << "min point: " << min_point << std::endl;
    else
        std::cout << "max point: " << min_point << std::endl << "min point: " << max_point << std::endl;
    return 0;
}