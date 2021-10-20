#ifndef GRAPH_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define GRAPH_H

#include <vector>

using namespace std;

class Graph {

    public:
        /**
         * Number of nodes in graph
         */
        int n; 
        /**
         * Adjacency list
         */
        vector<vector<int>> adj;

    Graph(vector<vector<int>> adj_lst) {
        adj = adj_lst;
        n = adj.size();
    }

    /**
     * Returns true if the edge (u,v) is in E(G).
     */
    bool has_edge(int u, int v);
};

//TODO: Ikke sikkert jeg trenger så mye mer her ennå.


#endif
