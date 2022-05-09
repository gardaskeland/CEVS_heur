#ifndef BOOKKEEP_H
#define BOOKKEEP_H

#include <set>
#include "b_merge.h"
#include "../utility/segment_tree.h"
#include "b_add_node.h"
#include "b_split.h"
#include "b_lp.h"
#include "b_perturbation.h"
#include "map"
#include <queue>

using namespace std;

class BRemoveAdd {
    public:
        vector<pair<int, tuple<int, int, int>>> best_p3s;
        int best_p3s_counter;
        //node from set to set
        vector<pair<int, int>> next_move_remove;
        vector<pair<int, int>> next_move_add;
        int index;
        int counter;
        bool initiated;
        int solution_hash;

        BRemoveAdd() {
            counter = 0;
            initiated = false;
        }
};

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
        int last_heuristic;
        SegmentTree<int> modified_clusters;
        SegmentTree<int> modified_vertices;
        BMerge b_merge;
        BAddNode b_add_node;
        BSplit b_split;
        BLP b_lp;
        BPerturbation b_perturbation;
        BRemoveAdd b_remove_add;
        int current_cost;

    /**
     * Remember to initialise co-occurence in accordance with initial solution.
     */
    Bookkeep(int operations) {
        total_operations = operations;
        operation_number = 0;
        SegmentTree<int> sgt(operations);
        modified_clusters = sgt;
        modified_vertices = sgt;
        b_remove_add.initiated = false;
        b_remove_add.counter = 0;
        BRemoveAdd();
    }

    Bookkeep() {
        operation_number = 0;
        SegmentTree<int> sgt(1);
        modified_clusters = sgt;
        modified_vertices = sgt;
        BRemoveAdd();

    }

};

#endif