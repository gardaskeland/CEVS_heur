#ifndef SPLIT_H
#define SPLIT_H

#include <map>
#include <queue>
#include <algorithm>
#include <set>
#include "../graph.h"
#include "../solution_representation.h"

using namespace std;

/**
 * @brief Computes the inner cost of a set in the solution representation, that is
 * (sum of cost of edge additions in set si) / |set si|.
 * 
 * @param g 
 * @param sol 
 * @param si 
 * @return int 
 */
int compute_inner_cost (Graph &g, SolutionRepresentation &sol, int si);

/**
* Stores info about a split that may be executed in sol.book.b_split and returns
* the cost of executing this split. It splits a set with as low inner cost as
* possible and uses Karger's algorithm to find an optimal way to split it.
*/
int greedy_split(Graph &g, SolutionRepresentation &sol);

#endif
