#ifndef GREEDY_MERGE_H   // To make sure you don't declare the function more than once by including the header multiple times.
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

void merge(SolutionRepresentation sol, Graph g, int si, int sj);

void greedy_merge(SolutionRepresentation sol, Graph g);


#endif