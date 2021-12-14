#ifndef SOLUTION_REPRESENTATION_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define SOLUTION_REPRESENTATION_H

#include <map>
#include <set>
#include <vector>
#include <iterator>
#include <tuple>
#include "graph.h"
#include "bookkeep/bookkeep.h"
#include "utility/segment_tree.h"
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

class SolutionRepresentation
{
    public: 
        int number_nodes;
        // Enumerated sets of nodes corresponding to clusters
        map<int, set<int>> clusters;
        // Enumerated nodes mapped to the clusters they are in.
        map<int, set<int>> node_in_clusters;
        // Stores calculations from operations so that we don't need to repeat them and
        // the values defining how to execute an operation we how found the cost of executing.
        Bookkeep book;
    
    SolutionRepresentation() {
        clusters = map<int, set<int>>();
        node_in_clusters = map<int, set<int>>();
        Bookkeep b(1);
        book = b;
    }

    SolutionRepresentation(int operations) {
        clusters = map<int, set<int>>();
        node_in_clusters = map<int, set<int>>();
        Bookkeep b(operations);
        book = b;
    }

    /*
     *Sets the state to initial solution given number of nodes n in graph.
     */
    void initial_solution(int n);

    /**
     * @brief Modifies book according to modification of set si. That is, all sets
     * that have common vertices with si are added to modified_clusters in book.
     * 
     * @param si 
     */
    void changed_set(int si);

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
     * Gets a reference to the map storing the sets in the solution representation.
     */
    map<int, set<int>> get_clusters();

    /**
     * Gets a reference to the map storing the nodes mapped to their sets in the solution
     * representation.
     */
    map<int, set<int>> get_node_in_clusters();

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

    set<int> get_set_indices_as_set();

    /**
     * Return a tuple telling us the number of edge deletions, edge additions and
     * vertex splittings we need to get a cluster graph using the clusters given
     * by the solution representation.
     * 
     * TODO: Change so that it uses the weights of edges and vertices.
     */
    tuple<int, int, int> cost_operations(Graph &g);

    /**
     * Return the total number of operations needed to change graph G to the cluster graph
     * in the solution.
     */
    int cost_solution(Graph &g);

    /**
     * Creates an exact copy of the solution.
     */
    SolutionRepresentation copy_solution();

    void print_solution();

    void print_node_in_clusters();

};

void hey();

#endif