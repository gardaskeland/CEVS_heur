#ifndef ADD_NODE_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define ADD_NODE_H

#include "../graph.h"
#include "../bookkeep/bookkeep.h"
#include "../utility/weighted_random.h"
#include <set>
#include <utility>
#include <algorithm>
#include <vector>
#include <iostream>
#include <limits>
#include <optional>
#include "../solution_representation.h"

#define tri tuple<int, int, int>

using namespace std;

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
 * Returns a sorted vector of PAIRs (key, value). The keys are the value of S/N(v) - N(v)US, the estimated cost of adding v to S. The values
 * are the nodes v. The key-value PAIR that appears first is the best node to add to S.
 */
vector<pair<int, int>> best_nodes_to_add(Graph &g, SolutionRepresentation &sol, int si);

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
 * Finds a node v to add to set si. Returns the cost of this operation that may
 * be executed. If add_node_to_set_counter == 0, find the cost of adding all nodes
 * and update best_node_to_add_to_single_set. Else, pick the back tuple from this
 * array and add this node to the set.
 */
optional<int> add_node_to_set(Graph &g, SolutionRepresentation &sol);

/**
 * Finds a set that has not been changed for a while and tries to add a vertex to it.
 */
optional<int> add_node_to_set_unchanged(Graph &g, SolutionRepresentation &sol);

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
 * If remove_node_counter > 0, pick the last node from the best_nodes_to_remove vector
 * and which sets we want to remove it from in order to decrease cost. Else,
 * find the cost of removing all vertices so that the solution improves and
 * remove the best vertex. Store the results of the rest in best_node_to_remove
 * and sort it in descendgin order.
 */
optional<int> remove_node_accept(Graph &g, SolutionRepresentation &sol);

/**
 * Removes a node that have not been moved in a while.
 */
optional<int> remove_node_accept_unchanged(Graph &g, SolutionRepresentation &sol, int i);

/**
 * @brief Adds u to all its neighbours if it results in cost reduction.
 * 
 * @param g 
 * @param sol 
 * @param u 
 * @return int 
 */
int add_node_to_neighbours(Graph &g, SolutionRepresentation &sol, int u);

/**
 * @brief Uses add_node_to_neighbours on all nodes.
 * 
 */
int add_all_nodes_to_neighbours(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Finds a way to add a node to all the neighbour set so that the total cost of the solution
 * decreases. Stores this operation in b_add_node.v and b_add_node.sets_to_add_v_to. Calculates
 * for all nodes only every n/10 times it's called, else it pulls a vertex from sol.book.b_add_node.
 * vertices_to_add and finds what the cost of adding this vertec now is.
 * 
 * @param g 
 * @param sol 
 * @return optional<int> 
 */
optional<int> add_node_to_neighbours_accept(Graph &g, SolutionRepresentation &sol);

/**
 * Like add_node_to_neighbours_accept, but chooses a random vertex that has not been moved for a while.
 */
optional<int> add_node_to_neighbours_accept_unchanged(Graph &g, SolutionRepresentation &sol, int i);

/**
 * Checks all possibilities of adding k connected vertices to the set, where every vertex
 * is a neighbour of the set. Picks the best one.
 */
optional<int> add_k_to_set(Graph &g, SolutionRepresentation &sol, int si, int k);

/**
 * Picks the best operation when checking add_k_to_set for all sets.
 */
optional<int> add_k_to_a_set(Graph &g, SolutionRepresentation &sol, int k);


/**
 * @brief Apply the latest add-operation to sol.
 * 
 * @param sol 
 */
void do_add(SolutionRepresentation &sol);


#endif