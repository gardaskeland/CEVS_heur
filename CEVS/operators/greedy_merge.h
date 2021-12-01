#ifndef GREEDY_MERGE_H 
#define GREEDY_MERGE_H

#include "../solution_representation.h"
#include "../graph.h"
#include <set>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

/**
 * Returns c, the cost after merging si and sj minus cost before merging si and sj. If c < 0
 * we get a smaller cost function when merging.
 */
int merge_cost(SolutionRepresentation sol, Graph g, int si, int sj);

void merge(SolutionRepresentation sol, int si, int sj);

struct revert_greedy_merge;

//void do_revert_greedy_merge(SolutionRepresentation sol, revert_greedy_merge rgm);

void greedy_merge(Graph g, SolutionRepresentation &sol);


#endif