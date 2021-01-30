#include "pch.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <cstdio>
using namespace std;

char ignore_r(const char &c) {
    if (c < 'A' || c > 'Z')
        return c;
    return 'a' - 'A' + c;
}

bool alphavit(const char &ch) {
    if ((ch >= 'a' && ch <= 'z') || ch == '%' || (ch >= '0' && ch <= '9') || ch == '-' || ch == '+' || ch == '#')
        return true;
    return false;
}

void write_str(FILE* file, const string &str) {
    size_t tmp_val;
    tmp_val = str.size();
    fwrite(&tmp_val, sizeof(size_t), 1, file);
    for (size_t i = 0; i < str.size(); i++) {
        fwrite(&(str[i]), sizeof(char), 1, file);
    }
}

void read_str(FILE* file, string &str) {
    size_t size_str;
    char ch;
    fread(&size_str, sizeof(size_t), 1, file);
    str.resize(size_str);
    for (size_t i = 0; i < size_str; i++) {
        fread(&ch, sizeof(char), 1, file);
        str[i] = ch;
    }
}

struct SHB {
    size_t help1;
    size_t help2;
    size_t help3;
    SHB() {
        help1 = 0;
        help2 = 0;
        help3 = 0;
    }
};

int main(int argc, char* argv[])
{
    ios::sync_with_stdio(false);
    string text;
    size_t size_text;
    char ch;
    string filename_in = "C:\\Users\\Asus\\source\\repos\\KPDA\\Debug\\test.txt";
    string filename_out = "C:\\Users\\Asus\\source\\repos\\KPDA\\Debug\\bd.txt";
    if (argc == 1) {
        /*
        if ((string)argv[2] == "--input") {
            filename_in = argv[3];
            filename_out = argv[5];
        }
        else {
            filename_out = argv[3];
            filename_in = argv[5];
        }
        */
        size_t count = 0;
        size_t word_count;
        size_t t_count = 0;
        bool word = false;
        ifstream file_get(filename_in, ios::in);
        if (!file_get.is_open()) {
            cout << "ERROR: cant open input the file!\n";
            return 0;
        }
        file_get.tie(nullptr);
        unordered_map <string, size_t> words;
        unordered_map <string, unordered_map <string, size_t>> all(0);
        unordered_map <string, SHB> tags(0);
        while (true) {
            word_count = 0;
            if (!(file_get >> size_text))
                break;
            file_get.get();
            while (true) {
                ch = file_get.get();
                ch = ignore_r(ch);
                if (ch == '\'' && word) {
                    do
                        ch = file_get.get();
                    while (alphavit(ch));
                }
                if (ch == '#' && !word)
                    continue;
                if (alphavit(ch)) {
                    word = true;
                    text.push_back(ch);
                }
                else if (!alphavit(ch) && word) {
                    word = false;
                    word_count++;
                    words[text]++;
                    text.clear();
                }
                if (ch == '\n') {
                    size_text--;
                    if (size_text == 0)
                        break;
                    continue;
                }
            }
            do {
                ch = file_get.get();
                ch = ignore_r(ch);
                if (ch == ',' || ch == '\n') {
                    tags[text].help2++;
                    tags[text].help1 += word_count;
                    for (auto alloc = words.begin(); alloc != words.end(); alloc++)
                        all[alloc->first][text] += alloc->second;
                    text.clear();
                    if (ch == '\n') 
                        break;
                    else
                        continue;
                }
                text.push_back(ch);
            } while (true);
            count++;
            words.clear();
        }
        cout << 'a';
        ofstream file(filename_out, ios::out);
        unordered_map <string, size_t>::const_iterator use_tag;
        file << count << ' ' << all.size() << ' ' << tags.size() << '\n';
        for (auto alloc = tags.begin(); alloc != tags.end(); alloc++) {
            file << alloc->first << ' ' << alloc->second.help2 << ' ' << alloc->second.help1 << '\n';
            alloc->second.help3 = t_count;
            t_count++;
        }
        for (auto alloc = all.begin(); alloc != all.end(); alloc++) {
            file << alloc->first << ' ' << alloc->second.size() << '\n';
            for (auto alloc1 = alloc->second.begin(); alloc1 != alloc->second.end(); alloc1++) {
                file << '\t' << tags[alloc1->first].help3 << ' ' << alloc1->second << "\n";
            }
        }
        file.close();
        file_get.close();
    }
    else if (argc == 8) {
        string filename_get_base = "C:\\Users\\Asus\\source\\repos\\KPDA\\Debug\\bd.txt";
        for (int i = 0; i < argc; i++) {
            if ((string)argv[i] == "--stats")
                filename_get_base = argv[i + 1];
            if ((string)argv[i] == "--input")
                filename_in = argv[i + 1];
            if ((string)argv[i] == "--output")
                filename_out = argv[i + 1];
        }
        ofstream file(filename_out, ios::out);
        ifstream file_in(filename_get_base, ios::in);
        file_in.tie(nullptr);
        size_t D, V, t_size;
        unordered_map <string, size_t> Dc, L;
        unordered_map <string, unordered_map <string, size_t>> W;
        file_in >> D >> V >> t_size;
        if (D == 0)
            return 0;
        vector <string> help_tags(t_size);
        for (size_t i = 0; i < t_size; i++) {
            file_in >> text;
            help_tags[i] = text;
            file_in >> Dc[text] >> L[text];
        }
        size_t tmp_size, index;
        for (size_t i = 0; i < V; i++) {
            file_in >> text >> tmp_size;
            for (size_t j = 0; j < tmp_size; j++) {
                file_in >> index;
                file_in >> W[text][help_tags[index]];
            }
        }
        file_in.close();
        unordered_map <string, double> k1(0);
        unordered_map <string, double> k2(0);
        ifstream file_get(filename_in, ios::in);
        if (!file_get.is_open()) {
            cout << "ERROR: cant open the file!\n";
            return 0;
        }
        file_get.tie(nullptr);
        const double middle = 1 / (double)L.size();
        double help;
        while (true) {
            if (!(file_get >> size_text))
                break;
            file_get.get();
            for (auto alloc = L.begin(); alloc != L.end(); alloc++)
                k1[alloc->first] = log((double)Dc[alloc->first] / D);
            do {
                ch = file_get.get();
                ch = ignore_r(ch);
                if (!alphavit(ch)) {
                    for (auto alloc = k1.begin(); alloc != k1.end(); alloc++)
                        alloc->second += log((double)(W[text][alloc->first] + 1) / (V + L[alloc->first]));
                    text.clear();
                    if (ch == '\n') {
                        size_text--;
                        if (size_text == 0)
                            break;
                        continue;
                    }
                    while (true) {
                        ch = file_get.get();
                        ch = ignore_r(ch);
                        if (alphavit(ch) || ch == '\n')
                            break;
                    }
                    if (ch == '\n') {
                        size_text--;
                        if (size_text == 0)
                            break;
                        continue;
                    }
                }
                text.push_back(ch);
            } while (true);
            for (auto alloc = k1.begin(); alloc != k1.end(); alloc++) {
                help = 0;
                for (auto alloc1 = k1.begin(); alloc1 != k1.end(); alloc1++)
                    help += exp(alloc1->second - alloc->second);
                help = 1 / help;
                k2[alloc->first] = help;
            }
            bool tmp = false;
            for (auto alloc = k2.begin(); alloc != k2.end(); alloc++) {
                if (tmp) {
                    if (alloc->second > middle)
                        file << ',' << alloc->first;
                }
                else
                    if (alloc->second > middle) {
                        tmp = true;
                        file << alloc->first;
                    }
            }
            file << '\n';
        }
        file.close();
        file_get.close();
    }
    else
        cout << "WRONG FORMAT!\n";
    return 0;
}
