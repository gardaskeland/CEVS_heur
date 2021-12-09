#ifndef FIND_KERNEL_H
#define FIND_KERNEL_H

#include "../CEVS/weighted_graph.h"
#include "revert_kernel.h"

/**
 * @brief Finds a kernel on the CEVS-problem for input (G,k). Stores information
 * useful to turn a solution of the kernel into a solution of the original graph
 * in RevertKernel.
 * 
 * @param g 
 * @param revert 
 * @return WeightedGraph 
 */
WeightedGraph find_kernel(Graph &g, int &k, RevertKernel &revert);

/**
 * Returns the critical clique graph derived from G. Information about mapping
 * of nodes, removed isolated cliques etc. can be found in RevertKernel.
 * Can be optimized.
 */
WeightedGraph find_critical_clique_graph(Graph &g, RevertKernel &revert);

/**
 * Finds the connected components of graph g. Uses dfs with stack.
 */
vector<set<int>> find_connected_components(Graph &g);



#endif 