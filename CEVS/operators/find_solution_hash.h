#ifndef FIND_SOLUTION_HASH_H
#define FIND_SOLUTION_HASH_H

#include "../solution_representation.h"
#include "add_node.h"
#include "label_propagation.h"
#include <algorithm>

using namespace std;

int calculate_hash_add_node_to_neighbours(SolutionRepresentation &sol);

int calculate_hash_add_node_to_set(SolutionRepresentation &sol);

int calculate_hash_remove_node(SolutionRepresentation &sol);

int calculate_hash_label_propagation(SolutionRepresentation &sol);

#endif