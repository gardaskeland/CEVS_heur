#ifndef GREEDY_MERGE_H 
#define GREEDY_MERGE_H

#include "../solution_representation.h"
#include "../graph.h"
#include "../bookkeep/bookkeep.h"
#include "../utility/weighted_random.h"
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "../utility/segment_tree.h"
#include <queue>
#include <optional>
#include "label_propagation.h"

using namespace std;

/**
 * Returns c, the cost after merging si and sj minus cost before merging si and sj. If c < 0
 * we get a smaller cost function when merging.
 */
int merge_cost(SolutionRepresentation &sol, Graph &g, int si, int sj);

void merge(SolutionRepresentation &sol, int si, int sj);

void do_merge(SolutionRepresentation &sol);

struct revert_greedy_merge;

void do_revert_merge(SolutionRepresentation &sol, Bookkeep &book);

void greedy_merge(Graph &g, SolutionRepresentation &sol, Bookkeep &book);

/**
 * Finds a weighted random choice of best pairs of sets in sol to merge. Returns the
 * change in cost after executing the merge.
 */
int weighted_random_merge(Graph &g, SolutionRepresentation &sol);

/**
 * Every now and then, considers pairs of all sets that are neighbours and finds the cost
 * of merging them. Then tries to merge one of the best ones each time the operator is
 * executed.
 */
optional<int> fast_merge(Graph &g, SolutionRepresentation &sol);


#endif