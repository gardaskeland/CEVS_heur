#ifndef LABEL_PROPAGATION_H
#define LABEL_PROPAGATION_H

#include "../graph.h"
#include "../solution_representation.h"
#include "add_node.h"

using namespace std;


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
 * @brief Goes through every vertex in input order, removes the costliest copy
 *  and moves it to a neighbour cluster so that the cost of the solution reduces the most.
 * If it cannot be moved without increasing cost, it is not moved. Returns the cost reduction.
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int label_propagation_round(Graph &g, SolutionRepresentation &sol);

#endif