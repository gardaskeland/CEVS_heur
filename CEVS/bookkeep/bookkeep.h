#ifndef BOOKKEEP_H
#define BOOKKEEP_H

#include <set>
#include "../utility/segment_tree.h"
#include "b_add_node.h"
#include "b_lp.h"
#include "b_perturbation.h"
#include "map"
#include <queue>

using namespace std;

/**
 * Stores information about the current state of the algorithm. Currently,
 * stores how to revert the last change to current_solution. In the future,
 * it may store computations from operations that we don't need to repeat.
 */
class Bookkeep {
    public:
        int total_operations;
        int revert_add_node[2];
        int revert_merge_ind[2];
        set<int> revert_merge_sets[2];
        int operation_number;
        SegmentTree<int> modified_clusters;
        SegmentTree<int> modified_vertices;
        BAddNode b_add_node;
        BLP b_lp;
        BPerturbation b_perturbation;

    /**
     * Remember to initialise co-occurence in accordance with initial solution.
     */
    Bookkeep(int operations) {
        total_operations = operations;
        operation_number = 0;
        SegmentTree<int> sgt(operations);
        modified_clusters = sgt;
        modified_vertices = sgt;
    }

    Bookkeep() {
        operation_number = 0;
        SegmentTree<int> sgt(1);
        modified_clusters = sgt;
        modified_vertices = sgt;

    }

};

#endif