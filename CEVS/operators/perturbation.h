#ifndef PERTURBATION_H
#define PERTURBATION_H

#include "../graph.h"
#include "../solution_representation.h"
#include "add_node.h"
#include "label_propagation.h"

#define tri tuple<int, int, int>

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
 * Finds the inverse degree of coveredness of all vertices, that is how many of its neighbours it
 * does not cooccur with. Then returns the vertex with most neighbours not covered and tries to
 * add a set with three vertixes to the solution: That one and two of its neighbours.
 */
optional<int> add_trio(Graph &g, SolutionRepresentation &sol);

/**
 * Finds a set in the solution to remove.
 */
optional<int> remove_set_op(Graph &g, SolutionRepresentation &sol);

optional<int> swap(Graph &g, SolutionRepresentation &sol);

/**
 * @brief Picks a random set and adds a neighbour of one of its vertices to it.
 * 
 * @param g 
 * @param sol 
 * @return int 
 */
int add_adjacent_vertex(Graph &g, SolutionRepresentation &sol);

#endif