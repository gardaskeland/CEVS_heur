#ifndef B_ADD_NODE_H
#define B_ADD_NODE_H

#include <map>
#include <queue>
#include <algorithm>

using namespace std;


struct cmp {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.first > right.first;
    }
};

class BAddNode {
    public:
        //Stores the index of the node to be added to set si.
        int v = -1;

        //Stores the index of the set node v is to be added to.
        int si = -1;

        //Stores the relative out-degrees of sets. Mapped from set index to relative out-degree.
        map<int, int> relative_out_degrees;

        int last_add_operation;

        priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> pq_relative_out_degrees;

        void empty_pq();

        BAddNode() {
            last_add_operation = -1;
        }
};


#endif