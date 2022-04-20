#ifndef PY_INTERFACE
#define PY_INTERFACE

#include "graph.h"
#include "solution_representation.h"

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