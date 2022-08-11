#ifndef URH_H
#define URH_H

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
 * @brief Solves CEVS using uniform random choice of operators/subheuristics.
 * 
 * @param g 
 * @param num_operations 
 * @return LoggingSolution 
 */
LoggingSolution urh(Graph &input_g, LoggingSolution &log_sol, int num_operations, bool with_cc);

#endif