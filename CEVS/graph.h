#ifndef GRAPH_H   // To make sure you don't declare the function more than once by including the header multiple times.
#define GRAPH_H

#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <queue>

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

        vector<tuple<int, int, int>> all_p3s;

        /**
         * Number of edges in the graph. Must be set manually when using Graph() as constructor.
         */
        int num_edges;
    
    Graph() {
        adj = vector<vector<int>>();
        n = 0;
        num_edges = -1;
    }

    Graph(vector<vector<int>> adj_lst) {
        adj = adj_lst;
        n = adj.size();
        vector<int> to_add;
        int count_edges = 0;
        adj_matrix = vector<vector<int>>();
        all_p3s = vector<tuple<int, int, int>>();
        if (n < 30000) {
            adj_matrix = vector<vector<int>>(n);
        
            for (int i = 0; i < n; i++) {
                to_add = vector<int>(n, 0);
            
                for (int j : adj_lst[i]) {
                    to_add[j] = 1;
                    count_edges += 1;
                }
            
                for (int elm : to_add) {
                    adj_matrix[i].push_back(elm);
                }
            }
        }
        num_edges = count_edges / 2;
        /*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << adj_matrix[i][j] << " ";
            }
            cout << "\n";
        }
        */
        
    }

    /**
     * Returns true if the edge (u,v) is in E(G).
     */
    bool has_edge(int u, int v);

    /**
     * Returns the edge cost of edge u and v (1 for simple graph).
     */
    virtual int get_edge_cost(int u, int v);

    /**
     * Returns the node weight of node u in V(G) (1 for simple graph).
     */
    virtual int get_node_weight(int u);

    int get_number_nodes();

    /**
     * Find all paths of lengths 3 (not cycles) and stores them in all_p3s.
     */
    void find_all_p3s();

};


#endif
