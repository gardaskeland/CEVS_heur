#ifndef LABEL_PROPAGATION_H
#define LABEL_PROPAGATION_H

#include "../graph.h"
#include "../solution_representation.h"
#include "../bookkeep/bookkeep.h"
#include "add_node.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <random>
#include <iterator>
#include <iostream>
#include <tuple>
#include "../utility/weighted_random.h"

# define tri tuple<int, int, int>

using namespace std;

/**
 * @brief Stores the details of a move in label propagation: One vertex is moved from a set s0
 * to a set s1.
 * 
 */
struct lp_move {
    int v;
    int s0;
    int s1;
};

/**
 * @brief Returns the neighbour clusters of a vertex u. Here a neighbour cluster either is
 * some cluster that does not include the vertex, but does include
 * a neighbour of the vertex.
 * 
 * @param g 
 * @param sol 
 * @param u 
 * @return vector<int> 
 */
set<int> neighbour_clusters(Graph &g, SolutionRepresentation &sol, int u);


/**
 * @brief Suggests the best vertex move to do in the current state of the solution, from one set to another.
 * Returns the cost or a failed attempt and stores the details of the move in b_lp.
 * 
 * @param g 
 * @param sol 
 * @return optional<int> 
 */
//optional<int> best_vertex_move(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Goes through every vertex in input order, removes the costliest copy
 *  and moves it to a neighbour cluster so that the cost of the solution reduces the most.
 * If it cannot be moved without increasing cost, it is not moved. Returns the cost reduction.
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int label_propagation_round(Graph &g, SolutionRepresentation &sol);


/**
 * @brief Returns the best way to move vertex u. The returing triple contains
 * set to move from, set to move to and cost.
 * 
 * @param g 
 * @param sol 
 * @param u 
 * @return optional<tri> 
 */
optional<tri> find_best_move(Graph &g, SolutionRepresentation &sol, int u);

/**
 * @brief If label_prop_counter int sol.bookkeep is 0: Go through every vertex and find the cost of
 * moving one according to label propagation, then store all these in pq_label_prop in sol.book, sorted
 * by cost. Set the label_prop_counter to n/10 and try to execute the cheapest operation. Else, pick the cheapest
 * operation (as found previously), calculate the cheapest way to move it now and store this in sol.book.b_lp.
 * -1 as the set to move to means that the set to move to should be a new set in the solution.
 * 
 * @param g 
 * @param sol 
 * @return optional<int> 
 */
optional<int> label_propagation_accept(Graph &g, SolutionRepresentation &sol);

#endif