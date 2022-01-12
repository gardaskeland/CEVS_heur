#include "label_propagation.h"

set<int> neighbour_clusters(Graph &g, SolutionRepresentation &sol, int u) {
    set<int> to_return;
    for (int v : g.adj[u]) {
        for (int w : sol.get_node_to_clusters(v)) {
            set<int> set_w = sol.get_set(w);
            if (set_w.find(u) != set_w.end()) continue;
            to_return.insert(w);
        } 
    }
    return to_return;
}

int label_propagation_round(Graph &g, SolutionRepresentation &sol) {
    int cost = 0;
    for (int u = 0; u < g.n; u++) {
        int vertex_cost = 0;
        set<int> set_with_u = sol.get_node_to_clusters(u);
        int best_set_remove = -1;
        int best_set_cost = pow(2, 30);
        for (int si : set_with_u) {
            int set_cost = removal_cost(g, sol, si, u);
            if (set_cost < best_set_cost) {
                best_set_cost = set_cost;
                best_set_remove = si;
            }
        }
        if (best_set_remove == -1) continue;
        //Must find this before removal of u.
        set<int> neighbour_clusters_ = neighbour_clusters(g, sol, u);
        //sol.remove(u, best_set);
        vertex_cost += best_set_cost;

        int best_set_add = -1;
        best_set_cost = pow(2, 30);
        for (int si : neighbour_clusters_) {
            int set_cost = add_node_to_set_cost(g, sol, si, u);
            if (set_cost < best_set_cost) {
                best_set_cost = set_cost;
                best_set_add = si;
            }
        }

        //put in set by itself
        //cout << "best_set_cost: " << best_set_cost << "\n";
        if (best_set_cost > 0 && vertex_cost <= 0) {
            set<int> to_add;
            to_add.insert(u);
            sol.remove(u, best_set_remove);
            sol.add_set(to_add);
            //0 for adding, all cost measured in removal.
            cost += vertex_cost;
            continue;
        }
        
        //cout << "ok\n";
        vertex_cost += best_set_cost;
        //If it's cheapest to leave the vertex as is.
        if (vertex_cost > 0) continue;
        sol.remove(u, best_set_remove);
        sol.add(u, best_set_add);
        cost += vertex_cost;
    }
    return cost;
}