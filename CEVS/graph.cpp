#include "graph.h"

//Later make more efficient method using adjacency matrix?
bool Graph::has_edge(int u, int v) {
    for (int i : adj[u]) {
        if (i == v) {
            return true;
        }
    } return false;
}