#ifndef ADD_REMOVE_H
#define ADD_REMOVE_H

#include "add_node.h"
#include "../bookkeep/bookkeep.h"

using namespace std;

/**
 * Finds all paths of length 3 (not cycles) in graph g and stores them in all_p3s in g.
 */
//void find_all_p3s(Graph &g); should be a method in graph

/**
 * Chooses a p3 and tries to remove and reinsert it in the best way possible,
 * Now and then scans the graph to find the best p3s to move at a given point.
 */
optional<int> remove_add_3(Graph &g, SolutionRepresentation &sol);

/**
 * Like above, but the p3 is chosen randomly
 */
optional<int> remove_add_3_random(Graph &g, SolutionRepresentation &sol);

/**
 * Chooses a random p3 to remove and reinsert
 */
optional<int> simple_remove_add_3(Graph &g, SolutionRepresentation &sol);

/**
 * Every now and then scans a sample of p3s to remove and reinsert. Then
 * for a number of times afterwards tries to move the p3s that previously
 * were the best to move
 */
optional<int> sample_remove_add_3(Graph &g, SolutionRepresentation &sol);

void execute_remove_add_3(SolutionRepresentation &sol);

#endif