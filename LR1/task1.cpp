#include "iostream"
#include "vector"
#include <math.h>
#include <tuple>
#include <stdio.h>
#include <algorithm>
#include <queue>

using namespace std;
typedef long long ll;

//Проверяет, существует ли данная строка в списке кодов.
bool isCodsContainsWord(string s, vector<string>& cods) {
    for (int i = 0; i < cods.size(); i++) {
        if (s == cods[i]) {
            return true;
        }
    }
    return false;
}

//Определяет, может ли строка быть декодирована с использованием предоставленных кодов.
bool isCanDecode(string s, vector<string>& cods) {
    if (s == "") {
        return true;
    }
    bool flag = false;
    for (int i = 1; i <= s.length() && i < 7; i++) {
        if (isCodsContainsWord(s.substr(0, i), cods)) {
            flag = isCanDecode(s.substr(i), cods);
            if (flag) {
                return true;
            }
        }
    }
    return false;
}

//Кодирует строку на основе пользовательских кодов.
string encode(string s, vector<string>& cods) {
    string code = "";
    for (int i = 0; i < s.length(); i++) {
        code += cods[s[i] - 65];
    }
    return code;
}

int maxCount = -1;
//Пытаемся разделить закодированную строку на допустимые сегменты, которые могут быть декодированы.
void tryToSeparate(string s, vector<string>& cods, int count) {
    for (int i = 1; i <= s.length() && i < 7; i++) {
        if (!isCanDecode(s.substr(0, i), cods)) {
            if (!isCanDecode(s.substr(i), cods)) {
                if (maxCount < count + 1) {
                    maxCount = count + 1;
                }
            }
            tryToSeparate(s.substr(i), cods, count + 1);
        }
    }
}

int main()
{
    ll k;
    cin >> k;
    string s;
    cin >> s;
    vector<string> cods = vector<string>(k);
    for (int i = 0; i < k; i++) {
        string tmp;
        cin >> tmp;
        cods[i] = tmp;
    }
    tryToSeparate(encode(s, cods), cods, 1);
    cout << maxCount;
}