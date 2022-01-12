#ifndef LOCAL_SEARCH_LP_H
#define LOCAL_SEARCH_LP_H

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
#include "../operators/split.h"
#include "../operators/label_propagation.h"

using namespace std;

/**
 * @brief Performs a local search based on the label propagation operator with
 * add_node and remove_nodes as helping operators.
 * 
 * @param g 
 * @param num_operations 
 * @return ShallowSolution 
 */
ShallowSolution local_search_lp(Graph &g, int &num_operations);

#endif