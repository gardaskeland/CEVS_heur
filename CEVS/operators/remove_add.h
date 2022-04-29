#ifndef ADD_REMOVE_H
#define ADD_REMOVE_H

#include "add_node.h"
#include "../bookkeep/bookkeep.h"

using namespace std;

/**
 * Finds all paths of length 3 (not cycles) in graph g and stores them in all_p3s in g.
 */
//void find_all_p3s(Graph &g); should be a method in graph

optional<int> remove_add_3(Graph &g, SolutionRepresentation &sol);

#endif