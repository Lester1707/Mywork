#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <ctime>
using namespace std;

const size_t base = 9;
const size_t limit = 100000;

long long int Powl(const int &a, const unsigned int &n) {
    long long int r = 1;
    for (unsigned int i = 0; i < n; i++)
    {
        r *= a;
    }
    return r;
}

class TLongAr {
public:
    vector <long long int> value;
    bool exist;

    TLongAr (const string &num) {
        *this = num;
    }
    TLongAr () {
        exist = false;
    }
    TLongAr(const long long &num) {
        if (num < 0) {
            exist = false;
            return;
        }
        value.push_back(num);
        exist = true;
    }
    bool CheckMemory() {
        if (value.size() > limit / base) {
            exist = false;
            return true;
        }
        else {
            return false;
        }
    }

    TLongAr operator = (string num) {
        if (num.size() > limit) {
            return TLongAr();
        }
        if (num.size() == 0) {
            exist = false;
            return *this;
        }
        exist = true;
        while (num[0] == '0') {
            num.erase(num.begin());
            if (num.size() == 0) {
                value.push_back(0);
                return *this;
            }
        }
        size_t i = base - 1;
        do {
            if (i >= num.size()) {
                value.push_back(atoll(num.substr(0, base - (i - num.size()) - 1).c_str()));
                break;
            }
            value.push_back(atoll(num.substr(num.size() - 1 - i, base).c_str()));
            if (i == num.size() - 1)
                break;
            i += base;
        } while (true);
        return *this;
    }

    TLongAr operator = (const TLongAr &val2) {
        value = val2.value;
        exist = val2.exist;
        return *this;
    }

    TLongAr operator + (const TLongAr &value2) {
        TLongAr new_val;
        size_t n;
        if (value.size() > value2.value.size()) {
            n = value.size();
        } 
        else {
            n = value2.value.size();
        }
        long long int tmp;
        long long int plus = 0;
        for (size_t i = 0; i < n; i++) {
            if (i >= value.size())
                tmp = value2.value[i] + plus;
            else if (i >= value2.value.size())
                tmp = value[i] + plus;
            else
                tmp = value[i] + value2.value[i] + plus;
            plus = 0;
            if (tmp / Powl(10, base) > 0) {
                plus = tmp / Powl(10, base);
                new_val.value.push_back(tmp % Powl(10, base));
            }
            else {
                new_val.value.push_back(tmp);
            }
        }
        if (plus) {
            new_val.value.push_back(plus);
        }
        if (new_val.CheckMemory()) {
            return TLongAr();
        }
        new_val.exist = true;
        return new_val;
    }

    TLongAr operator - (const TLongAr &value2) {
        if (*this < value2) {
            return TLongAr();
        }
        TLongAr new_val;
        if (*this < value2) {
            return new_val;
        }
        long long int tmp;
        long long int minus = 0;
        for (size_t i = 0; i < value.size(); i++) {
            if (i >= value2.value.size()) {
                if (value[i] >= minus) {
                    tmp = value[i] - minus;
                    minus = 0;
                }
                else {
                    tmp = Powl(10, base) + value[i] - minus;
                    minus = 1;
                }
            }
            else {
                if (value[i] >= value2.value[i] + minus) {
                    tmp = value[i] - value2.value[i] - minus;
                    minus = 0;
                }
                else {
                    tmp = Powl(10, base) + value[i] - (value2.value[i] + minus);
                    minus = 1;
                }
            }
            new_val.value.push_back(tmp);
        }
        while (new_val.value[new_val.value.size() - 1] == 0) {
            new_val.value.erase(new_val.value.end() - 1);
            if (new_val.value.size() == 0) {
                new_val.value.push_back(0);
                break;
            }
        }
        new_val.exist = true;
        return new_val;
    }
    
    TLongAr operator * (const TLongAr &value2) {
        TLongAr new_val;
        long long cur;
        new_val.value = vector <long long>(value.size() + value2.value.size());
        for (size_t i = 0; i < value.size(); i++) {
            for (int j = 0, carry = 0; j < (int)value2.value.size() || carry; j++) {
                cur = new_val.value[i + j] + value[i] * 1ll * (j < (int)value2.value.size() ? value2.value[j] : 0) + carry;
                new_val.value[i + j] = cur % Powl(10, base);
                carry = cur / Powl(10, base);
            }
        }
        while (new_val.value.size() > 1 && new_val.value.back() == 0) {
            new_val.value.pop_back();
        }
        new_val.exist = true;
        return new_val;
    }

    TLongAr Shift(const size_t &begin, const size_t &end) {
        TLongAr new_val;
        new_val.exist = true;
        for (size_t i = end; i <= begin; i++)
            new_val.value.push_back(value[i]);
        return new_val;
    }

    TLongAr operator / (TLongAr &value2) {
        if (TLongAr("0") == value2) {
            return TLongAr();
        }
        if (*this < value2) {
            return TLongAr("0");
        }
        if (*this == value2) {
            return TLongAr("1");
        }
        if (value2.value.size() > 1) {
            TLongAr q_val;
            TLongAr tmp_val;
            TLongAr tmp2;
            long long b = Powl(10, base);
            long long d = b / (value2.value[value2.value.size() - 1] + 1);
            int m = value.size() - value2.value.size();
            size_t tmp = value.size();
            int n = value2.value.size();
            *this = *this * TLongAr(d);
            if (tmp == value.size()) {
                value.push_back(0);
            }
            value2 = value2 * TLongAr(d);
            int j = m;
            long long q;
            long long r;
            while (j >= 0) {
                q = (value[n + j] * b + value[n - 1 + j]) / value2.value[n - 1];
                r = (value[n + j] * b + value[n - 1 + j]) % value2.value[n - 1];
                while ((q == b || q * value2.value[n - 2] > b*r + value[n - 2 + j]) && r < b) {
                    q--;
                    r += value2.value[n - 1];
                }
                tmp = value2.value.size();
                tmp_val = value2 * TLongAr(q);
                tmp2 = Shift(j + n, j);
                if (tmp_val > tmp2) {
                    q--;
                    tmp_val = value2 * TLongAr(q);
                    if (tmp == tmp_val.value.size()) {
                        tmp_val.value.push_back(0);
                    }
                    tmp = tmp_val.value.size();
                    tmp2 = tmp2 - tmp_val;
                    while (tmp2.value.size() < n + 1) {
                        tmp2.value.push_back(0);
                    }
                    for (int i = j; i <= j + n; i++) {
                        value[i] = tmp2.value[i - j];
                    }
                }
                else {
                    if (tmp == tmp_val.value.size()) {
                        tmp_val.value.push_back(0);
                    }
                    tmp = tmp_val.value.size();
                    tmp2 = tmp2 - tmp_val;
                    while (tmp2.value.size() < n + 1)
                        tmp2.value.push_back(0);
                    for (int i = j; i <= j + n; i++) {
                        value[i] = tmp2.value[i - j];
                    }
                }
                q_val.value.insert(q_val.value.begin(), q);
                j--;
            }
            while (q_val.value[q_val.value.size() - 1] == 0) {
                q_val.value.erase(q_val.value.end() - 1);
                if (q_val.value.size() == 0) {
                    q_val.value.push_back(0);
                    return *this;
                }
            }
            q_val.exist = true;
            return q_val;
        }
        else {
            int carry = 0;
            for (int i = (int)value.size() - 1; i >= 0; --i) {
                long long cur = value[i] + carry * 1ll * Powl(10, base);
                value[i] = int(cur / value2.value[0]);
                carry = int(cur % value2.value[0]);
            }
            while (value.size() > 1 && value.back() == 0)
                value.pop_back();
            return *this;
        }
    }

    TLongAr operator ^ (const unsigned int &s) {
        if (s == 0) {
            if (*this == TLongAr("0")) {
                return TLongAr();
            }
            return TLongAr("1");
        }
        if (s == 1)
            return *this;
        vector <TLongAr> values;
        TLongAr r = *this;
        values.push_back(r);
        long long power = 1;
        long long next_pow = s;
        while (true)
        {
            r = r * r;
            if (r.CheckMemory()) {
                return TLongAr();
            }
            power *= 2;
            if (next_pow - power < power) {
                if (next_pow - power == 0) {
                    return r;
                }
                else if (next_pow - power == 1) {
                    return r * *this;
                }
                next_pow = next_pow - power;
                break;
            }
            values.push_back(r);
        }
        power = 1;
        size_t i = 0;
        while (true) {
            if (next_pow - power > power) {
                power *= 2;
                i++;
            } else if (next_pow == power) {
                r = r * values[i];
                break;
            }
            else if (next_pow - power <= power) {
                r = r * values[i];
                next_pow = next_pow - power;
                i = 0;
                power = 1;
            }
        }
        return r;
    }

    bool operator > (const TLongAr &value2) {
        if (value.size() > value2.value.size()) {
            return true;
        }
        else if (value.size() < value2.value.size()) {
            return false;
        }
        else {
            for (int i = value.size() - 1; i >= 0; i--) {
                if (value[i] > value2.value[i]) {
                    return true;
                }
                else if ((value[i] < value2.value[i])) {
                    return false;
                }
            }
        }
        return false;
    }

    bool operator < (const TLongAr &value2) {
        if (*this > value2 || *this == value2) {
            return false;
        }
        return true;
    }

    bool operator == (const TLongAr &value2) {
        if (value.size() == value2.value.size()) {
            for (int i = value.size() - 1; i >= 0; i--) {
                if (value[i] != value2.value[i]) {
                    return false;
                }
            }
        }
        else {
            return false;
        }
        return true;
    }

    void Print() {
        if (!exist) {
            cout << "Error\n";
            return;
        }
        if (value.size() == 0)
            return;
        cout << value[value.size() - 1];
        for (int i = (int)value.size() - 2; i >= 0; i--) {
            for (int j = base; j > 0; j--)
                if (value[i] / Powl(10, j - 1) == 0)
                    cout << '0';
                else {
                    cout << value[i];
                    break;
                }
        }
        cout << '\n';
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string num1;
    string num2;
    TLongAr val1;
    TLongAr val2;
    clock_t start;
    char ch;
    while (cin >> num1 >> num2 >> ch) {
        val1 = TLongAr(num1);
        val2 = TLongAr(num2);
        if (ch == '+') {
            start = clock();
            (val1 + val2).Print();
            cout << (double)(clock() - start)/CLOCKS_PER_SEC << " sec\n";
        }
        if (ch == '*') {
            start = clock();
            (val1 * val2).Print();
            cout << (double)(clock() - start) / CLOCKS_PER_SEC << " sec\n";
        }
        if (ch == '-') {
            start = clock();
            (val1 - val2).Print();
            cout << (double)(clock() - start) / CLOCKS_PER_SEC << " sec\n";
        }
        if (ch == '^') {
            start = clock();
            (val1 ^ atoi(num2.c_str())).Print();
            cout << (double)(clock() - start) / CLOCKS_PER_SEC << " sec\n";
        }
        if (ch == '/') {
            start = clock();
            (val1 / val2).Print();
            cout << (double)(clock() - start) / CLOCKS_PER_SEC << " sec\n";
        }
        if (ch == '>') {
            if (val1 > val2) {
                cout << "true\n";
            }
            else {
                cout << "false\n";
            }
        }
        if (ch == '<') {
            if (val1 < val2) {
                cout << "true\n";
            }
            else {
                cout << "false\n";
            }
        }
        if (ch == '=') {
            if (val1 == val2) {
                cout << "true\n";
            }
            else {
                cout << "false\n";
            }
        }
    }
    return 0;
}
