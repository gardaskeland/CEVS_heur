#include "graph.h"

bool Graph::has_edge(int u, int v) {
    if (adj_matrix[u][v] == 1) {
        return true;
    }
    return false;
}