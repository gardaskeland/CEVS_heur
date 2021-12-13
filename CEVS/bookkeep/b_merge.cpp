#include "b_merge.h"

void BMerge::empty_pq() {
    pq_merge_cost = priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, cmp2>();
}