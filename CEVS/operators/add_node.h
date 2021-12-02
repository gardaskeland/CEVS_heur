#ifndef ADD_NODE_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define ADD_NODE_H

#include "../solution_representation.h"
#include "../graph.h"
#include "../bookkeep/bookkeep.h"
#include "../utility/weighted_random.h"
#include <set>
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

/**
 * Returns the relative out degree of a set S in the current solution, that is outdeg(S)/|S|. Remains the same as
 * long as S is not changed.
 */ 
double relative_out_degree(Graph &g, SolutionRepresentation &sol, int si);


/**
 * Returns a map with relative out degrees of the sets mapped to sets.
 */
map<double, int> get_relative_out_degrees(Graph &g, SolutionRepresentation &sol);

/**
 * Returns the number of the set with the highest relative out-degree in the solution.
 */
int highest_relative_out_degree(Graph &g, SolutionRepresentation &sol);


/**
 * Returns a sorted map. The keys are the value of S/N(v) - N(v)US, the estimated cost of adding v to S. The values
 * are the nodes v. The key-value pair that appears first is the best node to add to S.
 */
map<int, int> best_nodes_to_add(Graph &g, SolutionRepresentation &sol, int si);


void do_revert_add_node(SolutionRepresentation &sol, Bookkeep &book);

/**
 * Chooses a set with high relative out-degree and finds neighbour node of the set that is added to the set.
 */
void add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book);

/**
 * Like the above, but uses weighted randomness to choose the set and neighbour.
 */
void weighted_random_add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book);


#endif