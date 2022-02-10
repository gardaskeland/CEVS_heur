#ifndef ADD_NODE_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define ADD_NODE_H

#include "../graph.h"
#include "../bookkeep/bookkeep.h"
#include "../utility/weighted_random.h"
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <limits>
#include <optional>
#include "../solution_representation.h"

using namespace std;

/**
 * Returns the relative out degree of a set S in the current solution, that is outdeg(S)/|S|. Remains the same as
 * long as S is not changed.
 */ 
double relative_out_degree(Graph &g, SolutionRepresentation &sol, int si);


/**
 * Updates the map with relative out degrees of the sets mapped to sets in sol.book.b_add_node.relative_out_degrees.
 * Pushes the updated and old relative out degrees on the pq for relative out degrees in book.b_add_node.
 */
void get_relative_out_degrees(Graph &g, SolutionRepresentation &sol);

/**
 * Returns the number of the set with the highest relative out-degree in the solution.
 */
int highest_relative_out_degree(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Returns the cost of adding node v to set si in the current solution.
 * 
 * @param g 
 * @param sol 
 * @param si 
 * @param v 
 * @return int 
 */
int add_node_to_set_cost(Graph &g, SolutionRepresentation &sol, int si, int v);

/**
 * @brief Returns the cost of removing node u from set si in the current solution.
 * 
 * @param g 
 * @param sol 
 * @param si 
 * @param u 
 * @return int 
 */
int removal_cost(Graph &g, SolutionRepresentation &sol, int si, int u);

/**
 * Returns a sorted vector of pairs (key, value). The keys are the value of S/N(v) - N(v)US, the estimated cost of adding v to S. The values
 * are the nodes v. The key-value pair that appears first is the best node to add to S.
 */
vector<pair<int, int>> best_nodes_to_add(Graph &g, SolutionRepresentation &sol, int si);

void do_revert_add_node(SolutionRepresentation &sol, Bookkeep &book);

/**
 * @brief Chooses a random set and finds the neighbour node of that set that gives the most reduction
 * in cost to add to the set. Returns the cost of adding this node and stores the information
 * needed to add it in sol.book.b_add_node.
 * 
 * @param g 
 * @param sol 
 * @param book 
 * @return int 
 */
optional<int> random_choice_add_node(Graph &g, SolutionRepresentation &sol);

//int random_choice_add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book)

/**
 * @brief Goes through all sets and finds the neighbour node of each that gives the most reduction
 * in cost to add to the set. Adds this node to the set. Returns the cost of adding this node and 
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int add_node_to_all(Graph &g, SolutionRepresentation &sol);

/**
 * Goes through a percentage of the sets and adds the neighbour that given the most cost reduction to each
 * set. Returns cost of adding this node. Give percentage between 0 and 100.
 */
int add_node_to_percent(Graph &g, SolutionRepresentation &sol, double percentage);

/**
 * Chooses a set with high relative out-degree and finds neighbour node of the set.
 * Returns the cost of adding the node to the set.
 */
int add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book);

/**
 * Like the above, but uses weighted randomness to choose the set and neighbour. Returns the cost of adding.
 */
int weighted_random_add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book);



/**
 * @brief Considers all vertices and removes them from every cluster that it costs more than it
 * gains for it to be in, down to one cluster.
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int remove_nodes_(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Apply the latest add-operation to sol.
 * 
 * @param sol 
 */
void do_add(SolutionRepresentation &sol);


#endif