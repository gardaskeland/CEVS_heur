#ifndef EVALUATE_H
#define EVALUATE_H

#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <set>
#include "../solution_representation.h"
#include "../graph.h"
#include <ctype.h>

/**
 * Takes graph g, a solution of CEVS on graph g and the ground truth of graph g.
 * Returns a vector with the OMNI-score, the omega-score and the majority accuracy.
 */
vector<double> evaluate_alns(Graph &g, SolutionRepresentation &sol, string ground_truth);

pair<double, double> find_majority_accuracy(Graph &g, SolutionRepresentation &calculate_sol, map<int, set<int>> &ground_truth);

#endif