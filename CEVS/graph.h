#ifndef GRAPH_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define GRAPH_H

#include <vector>
#include <iostream>

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

        vector<vector<int>> adj_matrix;

    Graph(vector<vector<int>> adj_lst) {
        adj = adj_lst;
        n = adj.size();
        vector<int> to_add;
        adj_matrix = vector<vector<int>>(n);
        for (int i = 0; i < n; i++) {
            to_add = vector<int>(n, 0);
            for (int j : adj_lst[i]) {
                to_add[j] = 1;
            }
            for (int elm : to_add) {
                adj_matrix[i].push_back(elm);
            }
        }
        /**
        *for (int i = 0; i < n; i++) {
        *    for (int j = 0; j < n; j++) {
        *        cout << adj_matrix[i][j] << " ";
        *    }
        *    cout << "\n";
        *}
        */
    }

    /**
     * Returns true if the edge (u,v) is in E(G).
     */
    bool has_edge(int u, int v);
};

//TODO: Ikke sikkert jeg trenger så mye mer her ennå.


#endif
