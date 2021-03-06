//#include "pch.h"
#include <iostream>
#include <vector>
using namespace std;

const long long int max_val = 10000000000000000;

struct TWay {
    int price;
    int vertex;
    TWay(const int &p, const int &v) {
        price = p;
        vertex = v;
    }
    TWay() {}
};

class TGraph {
private:
    vector <vector <TWay>> v;
    size_t size;
public:
    TGraph(const int &sz) : v(sz + 1) {
        size = sz;
    }
    TGraph(const int &sz, const int &sz2) : v(sz + 1, vector <TWay> (sz2)) {
        size = sz;
    }
    void AddWay(const int &out, const int &in, const int &pr) {
        v[out].push_back(TWay(pr, in));
    }

    void Algorithm(const int &start, const int &finish) {
        bool change;
        vector <long long int> dist(size + 1);
        for (size_t i = 1; i <= size; i++) {
            dist[i] = max_val;
        }
        dist[start] = 0;
        for (size_t k = 1; k < size; k++) {
            change = false;
            for (size_t j = 1; j <= size; j++) {
                for (size_t i = 0; i < v[j].size(); i++) {
                    if (dist[j] != max_val && dist[v[j][i].vertex] > dist[j] + v[j][i].price) {
                        change = true;
                        dist[v[j][i].vertex] = dist[j] + v[j][i].price;
                    }
                }
            }
            if (change == false) {
                if (dist[finish] == max_val) {
                    cout << "No solution\n";
                }
                else {
                    cout << dist[finish] << '\n';
                }
                return;
            }
        }
        if (dist[finish] == max_val) {
            cout << "No solution\n";
        }
        else {
            cout << dist[finish] << '\n';
        }
    }
};

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    int m;
    int start;
    int finish;
    int out;
    int in;
    int price;
    cin >> n >> m >> start >> finish;
    TGraph gr(n);
    for (int i = 0; i < m; i++) {
        cin >> out >> in >> price;
        gr.AddWay(out, in, price);
    }
    gr.Algorithm(start, finish);
    return 0;
}

