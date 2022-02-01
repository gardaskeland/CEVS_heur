#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

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
 * @brief Uses a simulated annealing metaheuristic to find a solution to CEVS.
 * Preprocessing used, where the critical clique graph is found.
 * 
 * @param g 
 * @param num_operations 
 * @return ShallowSolution 
 */
ShallowSolution simulated_annealing(Graph &g, int &num_operations);

#endif