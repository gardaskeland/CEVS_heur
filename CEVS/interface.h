#ifndef PY_INTERFACE
#define PY_INTERFACE

#include "graph.h"
#include "solution_representation.h"
#include "operators/add_node.h"
#include "operators/label_propagation.h"
#include "operators/find_solution_hash.h"
#include <vector>
#include "read_file.h"
#include "operators/greedy_merge.h"
#include "operators/split.h"
#include "operators/perturbation.h"

using namespace std;

/**
 * Contains functions that do the following:
 * - Find an operation on the solutionrepresentation and a hash-value
 * for the solutionrepresentation altered by the operation.
 * - Executes the last operation
 */

pair<int, int> find_heuristic(Graph &g, SolutionRepresentation &sol, int heuristic);

void execute_heuristic(SolutionRepresentation &sol);

int objective_function(Graph &g, SolutionRepresentation &sol);

Graph make_graph_from_gml(string filename);





#endif