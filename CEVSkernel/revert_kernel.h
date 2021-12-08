#ifndef REVERT_KERNEL_H
#define REVERT_KERNEL_H

#include <set>
#include <vector>
#include <map>
#include "../CEVS/weighted_graph.h"
#include "../CEVS/solution_representation.h"

/**
 * @brief Stores information that we use to transform a solution of the kernel
 * graph into a solution of the original graph. This includes isolated critical cliques
 * that have been removed.
 * 
 */
class RevertKernel {

    public:
    //Stores the number of critical cliques found.
    int num_cc;

    //Stores the k of the kernelized graph, k' <= f(k) for some function f.
    int k_marked;

    //All isolated critical cliques removed by the kernel algorithm.
    vector<set<int>> isolated_cc;

    //All non-isolated cc in.
    vector<set<int>> other_cc;

    //Not needed at the moment, as the first node corresponds to the first clique in
    //other_cc, the second to the second and i to i further.
    //Maps nodes in the weighted graph to corresponding critical cliques in other_cc.
    map<int, int> nodes_to_cc;

    //For cc in all_cc, stores the vertices that have been removed by reducing the cliques
    //to size k+1
    vector<set<int>> removed_from_cc;

    //Changes a weighted graph (as found by the kernel) and a solution into a solution for
    //the original graph.
    SolutionRepresentation transform_solution(WeightedGraph &g, SolutionRepresentation &sol);
};


#endif