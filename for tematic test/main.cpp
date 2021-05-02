#include <iostream>
#include <cmath>
#include <vector>

const uint32_t last_root = 2021;

float f(const float &x) { //логарифмируем модуль нашего многочлена, дабы избежать переполнения
    float sum = 0;
    for (uint32_t i = 1; i <= last_root; i++)
        sum += log(fabs(x - float(i)));
    return sum;
}

float f_d(const float &x) { //тогда производная
    float sum = 0;
    for (uint32_t i = 1; i <= last_root; i++)
        sum += 1.0/(x - float(i));
    return sum;
}

float find(float left, float right) {   //ищем точки где производная равна нулю. очевидно на интервале между двумя корнями исходного многочлена это точка есть
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
    std::cout << "Enter the front polinom coefficient: "; //многочлен всегда имеет некотырый передний коэффициент, 
    std::cin >> c;                                        //поэтому я решил он должен быть здесь, хотя он почти ни на что не влияет. 
    if (fabs(c) < __FLT_EPSILON__) {
        std::cout << "To small coefficient" << std::endl; //если он равен нулю это не многочлен
        throw;
    }
    std::vector <float> points(last_root - 1); //запоминаем будущие точки нуля производной
    for (uint32_t i = 1; i < last_root; i++)
        points[i - 1] = find(float(i), float(i + 1)); //ищем их на интервалах между корнями
    float max = 0.0;
    float max_point;
    float min_point;
    float num;
    for (uint16_t i = 0; i < last_root - 1; i += 2) { //так как функция будет, то опускаться, то подниматься от корня к корню (напоминая синус) то отрицательные и положительные локальные максимумы буду между собой чередоваться.
        num = f(points[i]);                           //поэтому рассмотрим последовательно все положительные максимумы и отрицательные максимумы (то есть минимумы)
        if (num > max) {                              //находим максимальное значение логарифмированной функции
            max = num;
            max_point = points[i];
        }
    }
    max = 0.0;
    for (uint16_t i = 1; i < last_root - 1; i += 2) { //здесь тоже, но для отрцательных пиков
        num = f(points[i]);
        if (num > max) {
            max = num;         
            min_point = points[i];
        }
    }
    if (c > 0.0)
        std::cout << "max point: " << max_point << std::endl << "min point: " << min_point << std::endl; //печатаем найденые максимум и минимум
    else
        std::cout << "max point: " << min_point << std::endl << "min point: " << max_point << std::endl; //если введеный коэффициент отрицателен, то максимум и минимум просто меняются местами.
    return 0;
}