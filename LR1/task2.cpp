#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int main()
{
    string s, s2, t; int i;
    vector <string> str;
    ifstream f("C://Users/vladc/OneDrive/Рабочий стол/input.txt"); 
    ofstream h("C://Users/vladc/OneDrive/Рабочий стол/output.txt");

    while (!f.eof())
    {
        getline(f, s);
        stringstream s1(s);
        while (s1 >> s2)
        {
            str.push_back(s2);
        }
        for (i = 0; i < str.size(); i++)
        {
            t = str[i];
            rotate(t.rbegin(), t.rbegin() + 3, t.rend());
            h << t;
        }
    }
    f.close();
    h.close();
}

