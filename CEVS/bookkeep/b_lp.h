#ifndef B_LP
#define B_LP

//#include "../operators/label_propagation.h"
#include <map>
#include <tuple>
#include <queue>
#include <utility>
#include <vector>

using namespace std;


struct cmp5 {
    bool operator() (pair<int, tuple<int, int, int>> &left, pair<int, tuple<int, int, int>> &right) {
        return left.first > right.first;
    }
};

class BLP {
    public:
        int s0;
        int v;
        int s1;
        //s0, v, si.
        map<tuple<int,int,int>, int> move_cost;

        priority_queue<pair<int, tuple<int, int, int>>, vector<pair<int, tuple<int, int, int>>>, cmp5> pq_move_cost;

        /**
         * @brief operations to change cost of after the stored move. Operations in tuple, cost change
         * 
         */
        vector<pair<tuple<int,int,int>, int>> after_move_change;

        /**
         * @brief Operations to delete after the stored move.
         * 
         */
        vector<tuple<int, int, int>> after_move_delete;
        
        //Counts the number of times two vertices occur in a set together. If a pair is not in the map, then 
        // the pair does not occur in a set togther (0 sets).
        map<pair<int, int>, int> co_occurence;

        BLP() {
            v = -1;
            s0 = 0;
            s1 = 0;

        }
        //maps from least cost of move to vertex to move.
        vector<pair<int, int>> best_label_prop;

        int label_prop_counter = 0;

        /**
         * @brief Stores the next move of label_propagation_accept. The first is the vertex to move
         * the second is the set to move from and the third is the set to move to.
         * 
         */
        tuple<int, int, int> next_move;
};

#endif