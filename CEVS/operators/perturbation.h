#ifndef PERTURBATION_H
#define PERTURBATION_H

#include "../graph.h"
#include "../solution_representation.h"
#include "add_node.h"
#include "label_propagation.h"

using namespace std;

/**
 * @brief Isolates a random vertex from a random set it is in. Returns the
 * cost of the operation.
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int vertex_isolator(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Moves a random vertex to a random neighbour cluster.
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int vertex_mover(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Randomly chooses two adjacent vertices in a set and isolates them
 * from the set (deleted from set, added to a new empty set).
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int clique_splitter(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Picks a random set and adds a neighbour of one of its vertices to it.
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int add_adjacent_vertex(Graph &g, SolutionRepresentation &sol);

#endif