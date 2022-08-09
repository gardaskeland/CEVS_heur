#ifndef ALNS_TIDY_H
#define ALNS_TIDY_H

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
#include "../operators/label_propagation.h"
#include "../operators/perturbation.h"
#include "../solution_representation.h"
#include "../utility/logging_solution.h"
#include "../operators/remove_add.h"

/**
 * @brief Uses a ALNS metaheuristic to find a solution to CEVS.
 * If with_cc is true, preprocessing is used, where the critical clique graph is found before the
 * normal algorithm is executed on this.
 * 
 * @param g 
 * @param num_operations 
 * @return ShallowSolution 
 */
LoggingSolution alns_final(Graph &g, LoggingSolution &log_sol, int &num_operations, bool with_cc);

#endif