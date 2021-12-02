#ifndef B_MERGE_H
#define B_MERGE_H

#include <map>
#include <utility>
#include <queue>
#include <vector>

using namespace std;

// https://en.cppreference.com/w/cpp/container/priority_queue 02.12.2021
//bool cmp(pair<int, pair<int, int>> left, pair<int, pair<int, int>> right)
//    {return left.first > right.first;};

struct cmp2 {
    bool operator() (pair<int, pair<int, int>> &left, pair<int, pair<int, int>> &right) {
        return left.first > right.first;
    }
};

class BMerge {
    public:
        // the index of one of the sets that may be merged
        int si;
        // the index of one of the sets that may be merged
        int sj;

        // Stores the cost of merging the sets with indices given in the pair
        map<pair<int, int>, int> map_merge_cost;
        // Sorts the pairs by lowest merge_cost. Need to check that the value is still the same in
        // map_merge_cost while popping.
        priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, cmp2> pq_merge_cost; 

        BMerge() {

        }

};

#endif