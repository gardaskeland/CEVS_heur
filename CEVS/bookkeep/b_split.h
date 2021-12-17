#ifndef B_SPLIT_H
#define B_SPLIT_H

#include <map>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;

struct cmp3 {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.first > right.first;
    }
};

class BSplit {
    public:
    //The next set to split
    int si;

    //How to split the set.
    pair<set<int>, set<int>> cut;
    //Maps each set to its most recently calculated inner cost.
    //Inner cost is (sum of cost of edges added between nodes in S)/|S|.
    map<int, int> inner_cost;

    //The index of the last operation that was a split.
    int last_split_operation;

    priority_queue<pair<int, int>, vector<pair<int, int>>, cmp3> pq_inner_cost;

    BSplit() {
        last_split_operation = -1;
    }
};

#endif