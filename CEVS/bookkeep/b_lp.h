#ifndef B_LP
#define B_LP

#include "../operators/label_propagation.h"
#include <map>
#include <tuple>
#include <queue>
#include <utility>
#include <vector>

struct cmp5 {
    bool operator() (pair<int, tuple<int, int, int>> &left, pair<int, tuple<int, int, int>> &right) {
        return left.first > right.first;
    }
};

class BLP {
    public:
        int v;
        int s0;
        int s1;
        map<tuple<int,int,int>, int> move_cost;

        priority_queue<pair<int, tuple<int, int, int>>, vector<pair<int, tuple<int, int, int>>>, cmp5> pq_move_cost;

        /**
         * @brief maps a move to the pairs of vertices [0 and 1] that must get changed [2] after the move.
         * 
         */
        map<tuple<int,int,int>, vector<tuple<int,int,int>>> after_move;
        
        //Counts the number of times two vertices occur in a set together. If a pair is not in the map, then 
        // the pair does not occur in a set togther (0 sets).
        map<pair<int, int>, int> co_occurence;

        BLP() {
            v = -1;
            s0 = 0;
            s1 = 0;
    }
};

#endif