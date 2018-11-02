#include <string>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
using namespace std;
const int MAXLEN = 10000;

string sub(string a, string b);
string add(string a, string b);
bool cmp(string &a, string & b);
long atol_x(string s) {
    char num[10000];
    int i;
    for (i = 0; i < s.size(); i++) {
        num[i] = s[i];
    }
    num[i] = '\0';
    return atol(num);
}

bool cmp(string &a, string &b) { // a, b 都是无符号的数，如果a>=b返回true，如果a<b返回false
    unsigned long la = a.size();
    unsigned long lb = b.size();
    int ia = 0;
    while (a[ia] == '0') {
        ia++;
    }
    int ib = 0;
    while (b[ib] == '0') {
        ib++;
    }
    int len1 = la-ia;
    int len2 = lb-ib;
    if (len1 > len2) return true;
    if (len2 > len1) return false;
    while (ia < la && ib < lb) {
        if (a[ia] < b[ib])
            return false;
        else if (a[ia] > b[ib])
            return true;
        ia++; ib++;
    }
    return false;
}
string add(string a, string b) {
    string res;
    int jin = 0;
    int ia = a.size()-1;
    int ib = b.size()-1;
    while (ia >=0 && ib>=0) {
        int ben = a[ia] + b[ib] - 2*'0' + jin;
        jin = ben >= 10? 1: 0;
        ben = ben%10;
        res = char(ben+'0') + res;
        ia--;ib--;
    }
    while (ia>=0) {
        int ben = a[ia] + jin-'0';
        jin = ben>=10?1:0;
        ben %= 10;
        res = char(ben+'0') + res;
        ia--;
    }
    while (ib >= 0) {
        int ben = b[ib] + jin-'0';
        jin = ben>=10?1:0;
        ben %= 10;
        res = char(ben+'0') + res;
        ib--;
    }
    if (jin)
        res = "1" + res;
    return res;
}

string sub(string a, string b) { // 简易的减法， a>b
    int ia = a.size()-1;
    int ib = b.size()-1;
    int jie = 0;
    string res;
    while (ib >= 0) {
        int ben = a[ia] -b[ib] - jie;
        if (ben < 0) {
            jie = 1;
            ben += 10;
        } else {
            jie = 0;
        }
        res = char(ben+'0') + res;
        ia--; ib--;
    }
    while (ia >= 0) {
        int ben = a[ia] - jie - '0';
        if (ben < 0) {
            jie = 1;
            ben += 10;
        } else
            jie = 0;
        res = char(ben+'0') + res;
        ia--;
    }

    return res;
}

string bnm(string n1, string n2) {
    long sa = n1.size();
    long sb = n2.size();
    long half = max(sa, sb) / 2;
    if (sa <= 2 || sb <= 2) { // 当两个数足够小的时候就可以进行常规的相乘了
        return to_string(atol_x(n1) * atol_x(n2));
    }
    string ra = n1.substr(0, sa-half);
    string rb = n1.substr(sa-half);
    string rc = n2.substr(0, sb-half);
    string rd = n2.substr(sb-half);
    
    string z2 = bnm(ra, rc);
    string z0 = bnm(rb, rd);
    string z1 = sub(sub(bnm(add(ra, rb), add(rc, rd)), z2), z0); // bnm(ra+rb, rc+rd) - z2 - z1;
    for (int i = 0; i < 2*half; i++) {
        z2 += "0";
    }
    for (int i = 0; i < half; i++) {
        z1 += "0";
    }
    
    return add(add(z2, z1),z0); // z2*pow1 + z1*pow2 + z0
}

string bigSub(string &a, string &b) {
    int minus = 0;
    if (a.size() == 0 || b.size() == 0) return "0"; // 处理为空的情况
    if ((a == "0" || a == "-0") && (b == "0" || b == "-0")) return "0"; // 处理等于0的情况
    if ((a[0] == '-' && b[0] !='-')) { // 处理一个为正一个为负
        a = a.substr(1);
        return "-" + add(a, b);
    } else if (a[0] != '-' && b[0] =='-') {// 处理一个为正一个为负
        b = b.substr(1);
        return add(a, b);
    } else if (a[0] == '-' && b[0] =='-') {// 处理两个为负
        a = a.substr(1);
        b = b.substr(1);
        minus = ~minus; // 用来记录一次符号变换，如果a>b，那么还是需要加上负号
    }
    string res;
    if (cmp(b, a)) {
        minus = ~minus; // 用来记录一次符号反转，如果b>a，那么需要加上负号，如果前面就已经有了符号就要去掉负号
        res = sub(b, a);
    } else {
        res = sub(a, b);
    }
    
    int i = 0;
    while (res[i] == '0' && i < res.size()-1) {
        i++;
    }
    if (minus)
        return "-" + res.substr(i);
    else
        return res.substr(i);
    
}

string bigAdd(string &a, string &b) {
    if (a.size() == 0 || b.size() == 0) return "0"; // 处理为空的情况
    if ((a == "0" || a == "-0") && (b == "0" || b == "-0")) return "0"; // 处理等于0的情况
    int minus = 0;
    if ((a[0] == '-' && b[0] !='-')) { // 处理一个为正一个为负
        a = a.substr(1);
        return bigSub(b, a);

    } else if (a[0] != '-' && b[0] =='-') {// 处理一个为正一个为负
        b = b.substr(1);
        return bigSub(a, b);
    } else if (a[0] == '-' && b[0] =='-') {// 处理两个为负
        a = a.substr(1);
        b = b.substr(1);
        minus = 1;
    }
    if (minus)
        return "-" + add(a, b);
    else
        return add(a, b);
}


string bigmultiply(string &a, string &b) {
    if (a.size() == 0 || b.size() == 0) return "0"; // 处理为空的情况
    if (a == "0" || a == "-0" || b == "0" || b == "-0") return "0"; // 处理等于0的情况
    int minus = 0;
    if ((a[0] == '-' && b[0] !='-')) { // 处理一个为正一个为负
        minus = 1;
        a = a.substr(1);
    } else if (a[0] != '-' && b[0] =='-') {// 处理一个为正一个为负
        minus = 1;
        b = b.substr(1);
    } else if (a[0] == '-' && b[0] =='-') {// 处理两个为负
        a = a.substr(1);
        b = b.substr(1);
    }
    if (a.size() > b.size()+1) { // 处理两个位数相差超过2的情况
        int i = (int)b.size();
        while (i < a.size() - 1) {
            b = "0" + b;
            i++;
        }
    } else if (a.size()+1 < b.size()) {
        int i = (int)a.size();
        while (i < b.size()-1) {
            a = "0" + a;
            i++;
        }
    }
    string res = bnm(a, b);
    int i = 0;
    while (res[i] == '0') {
        i++;
    }
    res = res.substr(i);
    if (minus)
        res = "-" + res;
    return res;
}

void test(int way) {
    srand(time(NULL));
    for (int i = 0; i < 10000; i++) {
        long a = rand() - 1000000000;
        long b =rand() - 1000000000;
        string sa = to_string(a);
        string sb = to_string(b);
        string sres = sub(sa, sb);
        long res = 0;
        switch (way) {
            case 1:
                res = a*b;
                sres = bigmultiply(sa, sb);
                break;
            case 2:
                res = a+b;
                sres = bigAdd(sa, sb);
                break;
            case 3:
                res = a-b;
                sres = bigSub(sa, sb);
                break;
            default:
                cout << "None" << endl;
                break;
        }
        
        cout << "a: " << a << ", b: " << b << endl;
        cout << "Normal: " << res << endl;
        cout << "String: " << sres << endl;
        if (to_string(res) != sres) {
            cout << "Error !!!" << endl;
            return;
        }
    }
    cout << "True!! " << endl;
    return;
}

void random_input_file(string path, int n) {
    ofstream os(path);
    if (!os.is_open()) return;
    os << n << endl;
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        int n1 = rand() % (i*i + i* 1000 + 100000) - (i*i + i* 1000 + 100000)/2;
        int n2 = rand() % (i*i + i* 1000 + 100000) - (i*i + i* 1000 + 100000)/2;
        int way = rand()%3 + 1;
        os << n1 << " " << n2 << " " << way << endl;
    }
    os.close();
}


int main() {
//    random_input_file("in2.dat", 20);
    int n = 0;
    cin >> n;
    for (int i =0; i < n ; i++) {
        string s1, s2;
        int way;
        cin >> s1 >> s2 >> way;
        switch (way) {
            case 1:
                cout << bigAdd(s1, s2) << endl;
                break;
            case 2:
                cout << bigSub(s1, s2) << endl;
                break;
            case 3:
                cout << bigmultiply(s1, s2) << endl;
                break;
            default:
                cout << "None" << endl;
                break;
        }
    }
    return 0;
}
