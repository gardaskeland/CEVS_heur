#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "../solution_representation.h"
#include "../graph.h"
#include "../operators/add_node.h"
#include "../operators/greedy_merge.h"
#include "../bookkeep/bookkeep.h"
#include <set>
#include <vector>
#include <utility>


/**
 * Uses a local search metaheuristic to find a solution for CEVS.
 * Operators: add_node and weighted_average greedy_merge.
 */
SolutionRepresentation local_search(Graph g);




#endif