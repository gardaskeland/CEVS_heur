
#include <iostream>
#include <cmath>

using namespace std;

int bin2(int n) {
    return max(1, (n * (n+1)) / 2);
}

/**
 * @brief Assumes s is less or equal to t. Finds the cost of the minimum solution
 * of CEVS on complete bipartite graphs.
 * 
 * @param s 
 * @param t 
 * @return int 
 */
int find_cost(int s, int t) {
    //cout << "Working on graph with bipartite sets of size " << s << " and " << t << "\n";
    if (s > t) return -1;
    int least_cost = pow(2, 30);
    for (int c = 1; c <= s; c++) {
        for (int d = 1; d <= t; d++) {
            int u = s / c;
            int rs = s % c;
            int v = t / d;
            int rt = t % d;
            int cost =  s*(d - 1) + t*(c - 1) + rs * bin2(u) + (c - rs) * bin2(u - 1) + \
                        rt * bin2(v) + (d - rt) * bin2(v - 1);
            if (cost < least_cost) {
                least_cost = cost;
            }
            //cout << "for c, d = " << c << ", " << d << ", we get the cost " << cost << "\n";
        }
    }
    return least_cost;
}

int main() {
    for (int i = 1; i < 10; i++) {
        for (int j = i; j < 10; j++) {
            cout << i << " " << j << " " << find_cost(i, j) << "\n";
        }
    }
}