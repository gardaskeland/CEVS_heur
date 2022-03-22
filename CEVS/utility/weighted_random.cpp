#include "weighted_random.h"

#include <cmath>
#include <algorithm>
#include <map>
#include <queue>
#include <iostream>

using namespace std;

int weighted_random_index(int max_index, int length, double log_base) {
    int k = min(max_index, length);
    //cout << "k= " << k <<"\n";
    int r = get_random_int() % ((int)pow(log_base, k)) + 1;
    //Using the change of log-base formula, giving us log_(log_base)(r).
    int floor_log = floor(log2(r)/log2(log_base));
    return max(0, k - floor_log - 1);
}

int get_random_int() {
    random_device rd;
    mt19937 gen(rd());
    return gen();
}

int main() {
    vector<int> count(10, 0);
    for (int i = 0; i < 1000000; i++) {
        count[weighted_random_index(10, 10, 2)]++;
    }
    for (int c : count) cout << c << " ";
    cout << "\n";

    for (int i = 0; i < 100; i++) {
        cout << get_random_int() << "\n";
    }
}
