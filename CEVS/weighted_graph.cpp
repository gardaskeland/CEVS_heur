#include "weighted_graph.h"

int WeightedGraph::get_node_weight(int v) {
    return node_weights[v];
}

int WeightedGraph::get_edge_cost(int u, int v) {
    return node_weights[u] * node_weights[v];
}

