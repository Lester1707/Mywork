#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

double MachineEpsilon() {
    double e = 1;
    while (1 + e / 2 > 1) {
        e /= 2;
    }
    return e;
}

double e = MachineEpsilon();

struct TPrice {
    int price;
    vector <int> str;
    int index;
    TPrice(const int &size) : str(size) {}
    bool operator < (const TPrice &pr) {
        if (price < pr.price) {
            return true;
        }
        return false;
    }
};

bool CheckLNS(vector <vector <double>> &out_matrix, const vector <int> &str) {
    vector <double> use(str.size());
    double k_pow = 0;
    for (size_t i = 0; i < str.size(); i++) {
        use[i] = (double)str[i];
    }
    for (size_t i = 0; i < out_matrix.size(); i++) {
        for (size_t j = i; j < out_matrix[i].size(); j++) {
            if (k_pow == 0 && out_matrix[i][j] != 0 && use[j] != 0) {
                k_pow = use[j] / out_matrix[i][j];
                use[j] = 0;
            }
            else if (k_pow == 0 && out_matrix[i][j] != 0 && use[j] == 0) {
                break;
            }
            else if (k_pow != 0) {
                use[j] -= out_matrix[i][j] * k_pow;
                if (abs(use[j]) <= e) {
                    use[j] = 0;
                }
            }
        }
        k_pow = 0;
    }
    int zero_counter = 0;
    for (size_t i = 0; i < use.size(); i++) {
        if (use[i] == 0) {
            zero_counter++;
        }
        else {
            break;
        }
    }
    if (zero_counter == use.size()) {
        return false;
    }
    out_matrix.push_back(use);
    return true;
}

bool Func(vector <TPrice> &matrix, vector <int> &answer, vector <vector <double>> &out, const int &n) {
    for (size_t i = 0; i < matrix.size(); i++) {
        if (CheckLNS(out, matrix[i].str)) {
            answer.push_back(matrix[i].index);
            if (answer.size() == n) {
                return true;
            }
        }
    }
    return false;
}

int main()
{
    int n;
    int m;
    int val;
    vector <vector <double>> out_matrix;
    vector <int> answer;
    cin >> m >> n;
    vector <TPrice> matrix(m, n);
    for (int i = 0; i < m; i++) { 
        for (int j = 0; j < n; j++) {
            cin >> val;
            matrix[i].str[j] = val;
        }
        matrix[i].index = i + 1;
        cin >> val;
        matrix[i].price = val;
    }
    clock_t start = clock();
    sort(matrix.begin(), matrix.end()); 
    if (Func(matrix, answer, out_matrix, n)) {
        sort(answer.begin(), answer.end());
        for (size_t i = 0; i < answer.size() - 1; i++) {
            cout << answer[i] << ' ';
        }
        if (answer.size()) {
            cout << answer[answer.size() - 1] << '\n';
        }
    }
    else {
        cout << "-1\n";
    }
    cout << (double)(clock() - start)/CLOCKS_PER_SEC << '\n';
    return 0;
}

