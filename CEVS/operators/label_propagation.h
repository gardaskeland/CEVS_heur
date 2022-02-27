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
 * @brief Label propagation round, but instead of moving a vertex only when cost reduces, it may also
 * move by cooling function.
 * 
 * @param g 
 * @param t 
 * @return int 
 */
int label_propagation_round_cool(Graph &g, SolutionRepresentation &sol, double t);

#endif