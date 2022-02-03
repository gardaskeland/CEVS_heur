#ifndef ALNS_H
#define ALNS_H

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
#include <optional>
#include"../operators/label_propagation.h"
#include "../solution_representation.h"

using namespace std;

/**
 * @brief Uses a ALNS metaheuristic to find a solution to CEVS.
 * Preprocessing used, where the critical clique graph is found.
 * 
 * @param g 
 * @param num_operations 
 * @return ShallowSolution 
 */
ShallowSolution alns(Graph &g, int &num_operations);

#endif