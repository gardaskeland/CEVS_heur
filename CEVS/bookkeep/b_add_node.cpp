#include "b_add_node.h"

void BAddNode::empty_pq() {
    pq_relative_out_degrees = priority_queue<pair<int, int>, vector<pair<int, int>>, cmp>();
}