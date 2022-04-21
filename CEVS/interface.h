#ifndef INTERFACE_H
#define INTERFACE_H

#include "graph.h"
#include "solution_representation.h"
#include "operators/add_node.h"
#include "operators/label_propagation.h"
#include "operators/find_solution_hash.h"

using namespace std;

/**
 * Contains functions that do the following:
 * - Find an operation on the solutionrepresentation and a hash-value
 * for the solutionrepresentation altered by the operation.
 * - Executes the last operation
 */

auto find_heuristic(Graph &g, SolutionRepresentation &sol, int heuristic);

void execute_heuristic(SolutionRepresentation &sol);



#endif