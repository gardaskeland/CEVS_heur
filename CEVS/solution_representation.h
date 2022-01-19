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
        vector<int> to_permute;
    
    SolutionRepresentation() {
        clusters = map<int, set<int>>();
        node_in_clusters = map<int, set<int>>();
        Bookkeep b(1);
        book = b;
        for (int i = 0; i < number_nodes; i++) {
            to_permute.push_back(i);
        }
    }

    /**
     * @brief Construct a new Solution Representation object
     * 
     * @param n - number of nodes in the graph
     * @param operations 
     */
    SolutionRepresentation(int n, int operations) {
        number_nodes = n;
        clusters = map<int, set<int>>();
        node_in_clusters = map<int, set<int>>();
        Bookkeep b(operations);
        book = b;
        for (int i = 0; i < number_nodes; i++) {
            to_permute.push_back(i);
        }
    }

    /*
     *Sets the state to initial solution given number of nodes n in graph.
     */
    void initial_solution(int n);

    /**
     * @brief Sets the state of the initial solution to be a complete graph.
     * 
     */
    void initial_solution_complete_graph(int n);

    /**
     * @brief Makes a cluster out of the closed neighbourhood of the maxdegree node and
     * repeats this until all vertices are included in a cluster. Clusters are disjoint.
     * Sets the state of the solution to this solution.
     * 
     * @param n 
     */
    void initial_solution_max_degree(Graph &g);

    
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
     * @brief Splits a set into the two disjoint sets set_1 and set_2.
     * 
     * @param si 
     * @param set_1 
     * @param set_2 
     */
    void disjoint_split(int si, set<int> &set_1, set<int> &set_2);

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
     * @brief Return average set size.
     * 
     */
    int avg_set_size();

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

    /**
     * @brief Returns the number of split operations needed to change the graph into the solution represented.
     * 
     * @return int 
     */
    int num_splits();

};

void hey();

#endif