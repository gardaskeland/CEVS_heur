#ifndef BOOKKEEP_H
#define BOOKKEEP_H

#include <set>
#include "b_merge.h"
#include "../utility/segment_tree.h"

using namespace std;

/**
 * Stores information about the current state of the algorithm. Currently,
 * stores how to revert the last change to current_solution. In the future,
 * it may store computations from operations that we don't need to repeat.
 */
class Bookkeep {
    public:
        int revert_add_node[2];
        int revert_merge_ind[2];
        set<int> revert_merge_sets[2];
        int operation_number;
        SegmentTree<int> modified_clusters;
        BMerge b_merge;

    Bookkeep(int operations) {
        operation_number = 0;
        SegmentTree<int> sgt(operations);
        modified_clusters = sgt;
    }

    Bookkeep() {
        operation_number = 0;
        SegmentTree<int> sgt(1);
        modified_clusters = sgt;

    }

};

#endif