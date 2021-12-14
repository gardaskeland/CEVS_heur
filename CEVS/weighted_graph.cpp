#include "weighted_graph.h"

int WeightedGraph::get_node_weight(int v) {
    //cout << "get node weight in sublass accessed\n";
    return node_weights[v];
}

int WeightedGraph::get_edge_cost(int u, int v) {
    //cout << "get edge cost in subclass accessed\n";
    return node_weights[u] * node_weights[v];
}

