#include<vector>
#include<ctime>
#include<cmath>
#include<algorithm>
#include<iostream>
#include<Windows.h>
#include<cstdlib>

using namespace std;
using ll = long long;

ll divisor;

vector<double> r;
// vector<ll> Price{ 0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 };
vector<double> Price{ 0, 1, 7, 6, 10, 13, 11, 19, 14, 17, 31 };
vector<double> averagePrice;
double alphaPrice, betaPrice;
int alphaLen, betaLen;
int n, N, limit, m;

int getRandNum(int a, int b, int seed) {
    // 取得[a,b)的随机整数：
    srand((unsigned int)time(NULL) * seed);
    return rand() % (b - a) + a;
}

void showPrice() {
    cout << "Price List will be shown below" << endl;
    for (int i = 1;i < Price.size();i++)
        cout << Price[i] << " ";
    cout << endl;
}

void getBaseInfo(bool isDefault, int i, int scale) {
    if (isDefault) {
        cout << "Now is default pattern" << endl;
        // n = Price.size() - 1;
        // N = 1e9 + 5;
        n = getRandNum(10, 50, i);
        N = getRandNum((int)pow(10, scale), (int)(pow(10, scale + 1) - 1), i);
        Price.clear();Price.push_back(0);
        for (int k = 1;k <= n;k++) {
            Price.push_back(getRandNum(1, 100, i) * 1.0 / 10);
        }
    }
    else {
        cout << "n represent the size of priceArray" << endl;
        cout << "N represent the size of targetNum" << endl;
        cout << "please input n: "; cin >> n;
        cout << "Please input N: "; cin >> N;
        Price.clear();Price.push_back(0);
        // for (int i = 1; i <= n; i++) {
        //     ll t;
        //     cin >> t;
        //     Price.push_back(t);
        // }
        Price.clear();Price.push_back(0);
        for (int k = 1;k <= n;k++) {
            Price.push_back(getRandNum(1, 100, k) * 1.0 / ::divisor);
        }
    }
    r = vector<double>(N + 1);
    fill(r.begin(), r.end(), 0);
    showPrice();
}

void initAverageProfit() {
    averagePrice.push_back(0.0);
    alphaPrice = 0.0, betaPrice = -1.0;
    for (int i = 1;i <= n;i++) {
        averagePrice.push_back(Price[i] * 1.0 / i);
    }
    vector<double>temp(averagePrice);
    sort(temp.begin(), temp.end());
    while (true) {
        alphaPrice = temp[temp.size() - 1];
        betaPrice = temp[temp.size() - 2];
        auto maxIter = find(averagePrice.begin(), averagePrice.end(), alphaPrice);
        auto secIter = find(averagePrice.begin(), averagePrice.end(), betaPrice);
        alphaLen = maxIter - averagePrice.begin();
        betaLen = secIter - averagePrice.begin();
        if ((alphaPrice - betaPrice < 1e-5) && alphaLen % betaLen == 0) {
            averagePrice.erase(maxIter);
            temp.erase(temp.end() - 1);
        }
        else break;
    }
    m = N % alphaLen;
}

void setLimit() {
    limit = (betaPrice * m - r[m]) / ((alphaPrice - betaPrice) * alphaLen) + 1;
}

bool useStaticProgramming() {
    if (alphaPrice - betaPrice < 1e-12) {
        cout << "maxAverage equals secAverage" << endl;
        return false;
    }
    if (limit > N) return false;
    return true;
}

void DP(int to) {
    double temp;
    for (int i = 1;i <= to;i++) {
        temp = -1;
        for (int j = 1;(j <= i) && (j <= n);j++)
            temp = max(temp, Price[j] + r[i - j]);
        r[i] = temp;
    }
}

ll StaticProgramming() {
    DP(n);
    initAverageProfit();
    setLimit();
    if (useStaticProgramming() == false) {
        DP(N);
        return r[N];
    }
    // Price.erase(Price.begin() + alphaLen);
    Price[alphaLen] = 0;
    fill(r.begin(), r.begin() + limit * alphaLen + m + 1, 0);
    DP(limit * alphaLen + m);
    ll maxProfit = (N / alphaLen) * alphaLen * alphaPrice + r[m];
    ll temp = (N / alphaLen) * alphaLen * alphaPrice;
    for (int k = 1;k <= limit;k++) {
        maxProfit = max(maxProfit, (ll)(temp - k * alphaLen * alphaPrice + r[m + k * alphaLen]));
    }
    return maxProfit;
}

void DPTest() {
    fill(r.begin(), r.end(), 0);
    // clock_t start = clock();
    LARGE_INTEGER freq;
    LARGE_INTEGER start_t, stop_t;
    double time;
    QueryPerformanceCounter(&freq);
    QueryPerformanceCounter(&start_t);
    DP(N);
    ll ans = r[N];
    // clock_t end = clock();
    // double time = (end - start) * 1.0 / CLK_TCK;
    QueryPerformanceCounter(&stop_t);
    time = 1e3 * (stop_t.QuadPart - start_t.QuadPart) / freq.QuadPart;
    cout << "Use DP, the ans is " << ans << ", time cost: " << time << "s" << endl;
}

void SPTest() {
    fill(r.begin(), r.end(), 0);
    // clock_t start = clock();
    LARGE_INTEGER freq;
    LARGE_INTEGER start_t, stop_t;
    double time;
    QueryPerformanceCounter(&freq);
    QueryPerformanceCounter(&start_t);
    ll ans = StaticProgramming();
    // clock_t end = clock();
    // double time = (end - start) * 1.0 / CLK_TCK;
    QueryPerformanceCounter(&stop_t);
    time = 1e3 * (stop_t.QuadPart - start_t.QuadPart) / freq.QuadPart;
    cout << "Use SP, the ans is " << ans << ", time cost: " << time << "s" << endl;
}

int main() {
    cout << "请输入divisor:(divisor越大，最大和次大的平均利润差最小，新算法的性能越低)" << endl;
    cout << "但是只有当divisor > 1e9时，新算法的性能才会低于动态规划算法" << endl;
    cin >> ::divisor;
    getBaseInfo(false, 1, 6);
    DPTest();
    SPTest();
    system("pause");
    return 0;
}