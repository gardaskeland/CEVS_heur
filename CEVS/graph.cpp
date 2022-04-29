#include "graph.h"

bool Graph::has_edge(int u, int v) {
    if (adj_matrix.size() > 0 && adj_matrix[u][v] == 1) {
        return true;
    }
    else if (adj_matrix.size() == 0) {
        for (int w : adj[u]) {
            if (w == v) return true;
            }
        {
            /* code */
        }
    }
    return false;
}

int Graph::get_edge_cost(int u, int v) {
    return 1;
}

int Graph::get_node_weight(int u) {
    return 1;
}

int Graph::get_number_nodes() {
    return n;
}