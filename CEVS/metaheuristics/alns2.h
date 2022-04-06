#ifndef ALNS2_H
#define ALNS2_H

#include "../graph.h"
#include "../operators/add_node.h"
#include "../bookkeep/bookkeep.h"
#include "../utility/shallow_solution.h"
#include "../utility/segment_tree.h"
#include <set>
#include <vector>
#include <utility>
#include "../../CEVSkernel/find_kernel.h"
#include "../weighted_graph.h"
#include <optional>
#include "../operators/label_propagation.h"
#include "../operators/perturbation.h"
#include "../solution_representation.h"
#include "../utility/logging_solution.h"
#include <chrono>

using namespace std;

/**
 * @brief Uses a ALNS metaheuristic to find a solution to CEVS.
 * Preprocessing used, where the critical clique graph is found.
 * Current changes from alns:
 * Uses a version of label_propagation that utilizes acceptance criterea.
 * 
 * @param g 
 * @param num_operations 
 * @return ShallowSolution 
 */
LoggingSolution alns2(Graph &g, LoggingSolution &log_sol, int &num_operations);

LoggingSolution alns2_no_cc(Graph &g, LoggingSolution &log_sol, int &num_operations);

LoggingSolution test_label_propagation(Graph &g, LoggingSolution &log_sol, int &num_operations);

#endif