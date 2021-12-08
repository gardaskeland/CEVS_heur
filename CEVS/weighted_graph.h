#ifndef WEIGHTED_GRAPH_H
#define WEIGHTED_GRAPH_H

#include "graph.h"

/**
 * @brief Class for undirected graph with weighted nodes, so that an edge uv has weight
 * node_weight[u] * node_weight[v]. Used to model critical clique graphs.
 * 
 */
class WeightedGraph : public Graph {
    protected:
        vector<int> node_weights;
    
    public:

        // Gets the weight of the node.
        int get_node_weight(int v);

        /**
         * Since the graph is a critical clique graph, an edge uv has size node_weight[u] *
         * node_weight[v].
         * 
         * @param u 
         * @param v 
         * @return int 
         */
        int get_edge_cost(int u, int v);

        WeightedGraph(vector<vector<int>> &adj_lst, vector<int> &weights) {
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
            node_weights = weights;
        }
};

#endif