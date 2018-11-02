// 最近点对问题，给出二维平面上的n个点，求其中最近的两个点的距离。
// 算法描述：
// 运用分治法，首先把点按照x坐标的大小来进行排序，然后找到中间点mid，把点划分为两个区域分别进行查找
// 在合并的过程中，本来时间复杂度应该是o(n2)，但是对于中间线左边的点q，在中间线右边始终是落在2d*d的矩形区域内
// 所以通过鸽巢原理，最多只有检索常数个点--6个，把带状区域内的点按y坐标的大小从大到小排序，然后依次检索
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>
#include <map>

using namespace std;

const double MAX = 1e10; //记录最大距离
#define LEN 3000000
#define SIZE 100

struct node {
    int x,y;
};
vector<node> st_nodes;
vector<node> en_nodes;

node ar[LEN], br[LEN];
bool cmpx(node &a, node &b) { return a.x < b.x;} // 按x坐标升序排列
bool cmpy(node &a, node &b) { return a.y < b.y;} // 按y坐标升序排列
// 求两点距离 勾股定理
double dis(node &a, node &b) {
    return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}

double sld(int st, int en) {
    int mid = 0; // 用于记录中间位置
    int tail = 0; // 用于记录br的大小
    if (st == en) return MAX; // 如果只有一个元素，那么返回无穷大
    double d = MAX;
    mid = (st + en) / 2;
    double d1 = sld(st, mid);
    double d2 = sld(mid+1, en);
    d = min(d1, d2);
//    d = min(sld(st, mid), sld(mid+1, en)); // 首先在两边分别查找
    
    // 然后考虑跨越中间线的点对
    for (int i = mid; i >= st && ar[mid].x-ar[i].x < d; i--) { // 首先找到带状区域里面靠左的点
        br[tail++] = ar[i];
    }
    for (int i = mid+1; i < en && ar[i].x-ar[mid].x < d; i++) { // 然后找到带状区域里面靠右的点
        br[tail++] = ar[i];
    }
    sort(br, br+tail, cmpy); // 按y坐标升序排列
    for (int i = 0; i < tail; i++) {
        for (int j = i+1; j < tail && br[j].y-br[i].y <= d; j++) { // 根据鸽巢原理最多遍历6个点
            if (d >= dis(br[i], br[j])) {
                d = dis(br[i], br[j]);
                if (st_nodes.empty()) {
                    st_nodes.push_back(br[i]);
                    en_nodes.push_back(br[j]);
                } else {
                    double ld = dis(st_nodes.back(), en_nodes.back());
                    if (d < ld) {
                        st_nodes.clear();
                        en_nodes.clear();
                        st_nodes.push_back(br[i]);
                        en_nodes.push_back(br[j]);
                    } else if (d == ld) {
                        st_nodes.push_back(br[i]);
                        en_nodes.push_back(br[j]);
                    }
                }

            }
        }
    }
    return d;
}

void printGrid(int pn) {
    bool x_num[SIZE][SIZE];
    memset(x_num, 0, sizeof(x_num));
    for (int i = 0; i < pn; i++) {
        x_num[ar[i].x+SIZE/2][ar[i].y+SIZE/2] = true;
    }
    cout << " \t";
    for (int i = 0; i < SIZE; i++) {
        cout << i -SIZE/2<< "\t";
    }
    cout << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << i- SIZE/2 << "\t";
        for (int j = 0; j < SIZE; j++) {
            if (x_num[j][i])
                cout << "*\t";
            else cout << " \t";
        }
        cout << endl;
        cout << endl;
    }
}
map<int, bool> x_p, y_p;

void random_point_file(string path, int n) {
    ofstream os(path);
    if (!os.is_open()) return ;
    os << n << endl;
    srand((unsigned)time(NULL));
    for (int k = 0; k<n;k++) {
        x_p.clear();
        y_p.clear();
        int rt = 0;
        string res;
        int np = rand() % (5* k+3);
        for (int i = 0; i < np; i++) {
            int x = rand() % (2*i+10) - (2*i+10)/2;
            int y = rand() % (3*i+3) - (3*i+3)/2;
            if (x_p.count(x) == 0 || y_p.count(y) == 0) {
                res += to_string(x) + " " + to_string(y) + "\n";
                x_p[x] = true;
                y_p[y] = true;
                rt++;
            }
        }
        res = to_string(rt) + "\n" + res;
        os << res;
    }
    os.close();
}
void random_point(int &pn) {
    srand((unsigned)time(NULL));
    int rt = 0;
    for (int i = 0; i < pn; i++) {
        int x = rand() % SIZE - SIZE/2;
        int y = rand() % SIZE - SIZE/2;
        if (x_p.count(x) == 0 || y_p.count(y) == 0) {
            ar[rt].x = x;
            ar[rt].y = y;
            x_p[x] = true;
            y_p[y] = true;
            rt++;
        }
    }
    pn = rt;
}
double primeF(int &pn) {
    double d = MAX;
    for (int i = 0; i < pn; i++) {
        for (int j = i+1; j < pn; j++) {
            if (dis(ar[i], ar[j]) < d)
                d = dis(ar[i], ar[j]);
        }
    }
    return d;
}
//int main() {
//    int n = 0;
//    cin >> n;
//    while (n--) {
//        int np = 0;
//        cin >> np;
//        for (int i = 0; i < np; i++) {
//            cin >> ar[i].x >> ar[i].y;
//        }
//        sort(ar, ar+np, cmpx);
//        cout << sld(0, np) << endl;
//        for (int i = 0; i < st_nodes.size(); i++) {
//            cout << st_nodes[i].x << " " << st_nodes[i].y << endl;
//            cout << en_nodes[i].x << " " << en_nodes[i].y << endl;
//        }
//        st_nodes.clear();
//        en_nodes.clear();
//    }
//    return 0;
//}

