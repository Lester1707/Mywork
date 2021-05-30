#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <cstdio>
using namespace std;

char ignore_r(const char& c) {
    if (c < 'A' || c > 'Z')
        return c;
    return 'a' - 'A' + c;
}

bool alphavit(const char& ch) {
    if ((ch >= 'a' && ch <= 'z') || ch == '%' || (ch >= '0' && ch <= '9') || ch == '-' || ch == '+' || ch == '#')
        return true;
    return false;
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
    string filename_in = "C:\\Users\\kupts\\source\\repos\\BaiseTest\\Debug\\train.txt";
    string filename_out = "C:\\Users\\kupts\\source\\repos\\BaiseTest\\Debug\\bd.txt";
    string filename_get_base = filename_out;
    size_t count = 0;
    size_t word_count;
    size_t t_count = 0;
    bool word = false;
    ifstream file_get(filename_in, ios::in);
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
                if (ch != '\n')
                    do {
                        ch = file_get.get();
                    } while (ch != '\n' && ch != EOF);
                break;
            }
            text.push_back(ch);
        } while (true);
        count++;
        words.clear();
    }
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
    cout << "train size: " << count << endl;

    filename_out = "C:\\Users\\kupts\\source\\repos\\BaiseTest\\Debug\\answer.txt";
    filename_in = "C:\\Users\\kupts\\source\\repos\\BaiseTest\\Debug\\test.txt";
    file = ofstream(filename_out, ios::out);
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
    unordered_map <string, float> k1(0);
    unordered_map <string, float> k2(0);
    file_get = ifstream(filename_in, ios::in);
    if (!file_get.is_open()) {
        cout << "ERROR: cant open the file!\n";
        return 0;
    }
    file_get.tie(nullptr);
    string true_ans;
    float help;
    count = 0;
    size_t true_count = 0;
    unordered_map <string, size_t> pres;
    unordered_map <string, size_t> pres_tags;
    while (true) {
        if (!(file_get >> size_text))
            break;
        file_get.get();
        for (auto alloc = L.begin(); alloc != L.end(); alloc++)
            k1[alloc->first] = log((float)Dc[alloc->first] / D);
        do {
            ch = file_get.get();
            ch = ignore_r(ch);
            if (!alphavit(ch)) {
                for (auto alloc = k1.begin(); alloc != k1.end(); alloc++)
                    alloc->second += log((float)(W[text][alloc->first] + 1) / (V + L[alloc->first]));
                text.clear();
                if (ch == '\n') {
                    size_text--;
                    if (size_text == 0) {
                        do {
                            ch = file_get.get();
                            if (ch == ',' || ch == '\n')
                                break;
                            true_ans.push_back(ch);
                        } while (true);
                        while (ch != '\n' && ch != EOF)  {
                            ch = file_get.get();
                        }
                        break;
                    }
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
                    if (size_text == 0) {
                        do {
                            ch = file_get.get();
                            if (ch == ',' || ch == '\n')
                                break;
                            true_ans.push_back(ch);
                        } while (true);
                        while (ch != '\n' && ch != EOF) {
                            ch = file_get.get();
                        }
                    }
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
        float max = k2.begin()->second;
        string answer = k2.begin()->first;
        for (auto alloc = k2.begin(); alloc != k2.end(); alloc++) {
            if (alloc->second > max) {
                max = alloc->second;
                answer = alloc->first;
            }
        }
        pres_tags[true_ans]++;
        if (answer == true_ans) {
            true_count++;
            pres[true_ans]++;
        }
        file << answer << '\n';
        true_ans.clear();
        count++;
    }
    cout << "test size:" << count << endl;
    cout << "accuracy:" << float(true_count) / count << endl;
    cout << "tag | P | R\n";
    for (auto alloc = pres.begin(); alloc != pres.end(); alloc++) {
        cout << alloc->first << ' ' << float(alloc->second) / count << ' ' << float(alloc->second) / pres_tags[alloc->first] << '\n';
    }
    file.close();
    file_get.close();
    
    return 0;
}
