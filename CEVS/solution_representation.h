#ifndef SOLUTION_REPRESENTATION_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define SOLUTION_REPRESENTATION_H

#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <tuple>
#include "graph.h"
using namespace std;

class SolutionRepresentation
{
    public: 
        int number_nodes;
        // Enumerated sets of nodes corresponding to clusters
        map<int, set<int>> clusters;
        // Enumerated nodes mapped to the clusters they are in.
        map<int, set<int>> node_in_clusters;
        //TODO: Add datastructure counting the cost of e.g each node or cluster so
        //that we only update the costs of the part of the solution representation
        //we have changed
    
    SolutionRepresentation() {
        clusters = map<int, set<int>>();
        node_in_clusters = map<int, set<int>>();
    }

    /*
     *Sets the state to initial solution given number of nodes n in graph.
     */
    void initial_solution(int n);

    /**
     *Add node to set si
     */
    void add(int node, int si);

    /**
     *Remove node from set si
     */
    void remove(int node, int si);

    /**
     * Merge two sets
     */
    void merge(int si, int sj);

    /**
     * Gets a reference to a set si.
     */
    set<int> get_set(int si);

    /**
     * Gets a reference to the set telling which clusters node vi is in.
     */ 
    set<int> get_node_to_clusters(int vi);

    /**
     * Add a set to the solution representation.
     */
    void add_set(set<int> s);

    /**
     * Add a set with a specific index to the solution representation. Note that this replaces
     * the set that occupies the index.
     */
    void add_set_ind(int si, set<int> s);

    /**
     * Remove set si from the solution representation
     */
    void remove_set(int si);

    /**
     * Returns true if every node is in some set.
     */
    bool simple_feasibility_check();

    /**
     * Returns the numbers of sets in the solution.
     */
    int num_sets();

    /**
     * Obtain the indices of the sets currently in the solution.
     */
    vector<int> get_set_indices();

    /**
     * Return a tuple telling us the number of edge deletions, edge additions and
     * vertex splittings we need to get a cluster graph using the clusters given
     * by the solution representation.
     */
    tuple<int, int, int> cost_operations(Graph g);

    /**
     * Return the total number of operations needed to change graph G to the cluster graph
     * in the solution.
     */
    int cost_solution(Graph g);

    /**
     * Creates an exact copy of the solution.
     */
    SolutionRepresentation copy_solution();

    void print_solution();

};

void hey();

#endif