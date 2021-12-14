#ifndef LOCAL_SEARCH_H
#define LOCAL_SEARCH_H

#include "../solution_representation.h"
#include "../graph.h"
#include "../operators/add_node.h"
#include "../operators/greedy_merge.h"
#include "../bookkeep/bookkeep.h"
#include "../utility/shallow_solution.h"
#include "../utility/segment_tree.h"
#include <set>
#include <vector>
#include <utility>
#include "../../CEVSkernel/find_kernel.h"
#include "../weighted_graph.h"


/**
 * Uses a local search metaheuristic to find a solution for CEVS.
 * Operators: add_node and weighted_average greedy_merge.
 */
ShallowSolution local_search(Graph &g, int &num_operations);

/**
 * @brief Find the critical clique graph of g and uses a local search heuristic on this graph.
 * 
 * @param g 
 * @param num_operations 
 * @return ShallowSolution 
 */
ShallowSolution local_search_on_cc(Graph &g, int &num_operations);


#endif