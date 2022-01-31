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
    int original_cost = sol.cost_solution(g);
    vector<int> to_permute = sol.to_permute;
    //Shuffle from: https://en.cppreference.com/w/cpp/algorithm/random_shuffle 13.01.2021
    std::random_device rd;
    std::mt19937 gr(rd());
    shuffle(to_permute.begin(), to_permute.end(), gr);
    for (int u : to_permute) {
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

        sol.remove(u, best_set_remove);

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
        if ((best_set_cost > 0 && vertex_cost <= 0) || best_set_add == -1) {
            set<int> to_add;
            to_add.insert(u);
            sol.add_set(to_add);
            //0 for adding, all cost measured in removal.
            //+1 since we add back a node.
            cost += vertex_cost + 1;
            continue;
        }
        
        //cout << "ok\n";
        vertex_cost += best_set_cost;
        //If it's cheapest to leave the vertex as is.
        if (vertex_cost > 0) {
            //add the node back to original set
            sol.add(u, best_set_remove);
            continue;
        };
        sol.add(u, best_set_add);
        cost += vertex_cost;
        //cout << "for node " << u << ":\n";
        //sol.print_solution();
        //cout << "current cost: " << sol.cost_solution(g) << "\n";
        //cout << "estimated cost: " << original_cost + cost << "\n\n";
    }
    return cost;
}

void precompute_b_lp(Graph &g, SolutionRepresentation &sol) {
    //initialise cooccurence-map.
    for (int v = 0; v < g.n - 1; v++) {
        for (int u = v+1; u < g.n; u++) {
            int counter = 0;
            set<int> set_v = sol.get_node_to_clusters(v);
            set<int> set_u = sol.get_node_to_clusters(u);
            for (int i : set_v) {
                if (set_u.find(v) != set_u.end()) counter += 1;
            }
            if (counter > 0) {
                sol.book.b_lp.co_occurence[minmax(v, u)] = counter; 
            }
        }
    }

    //find cost of an operation and how it changes other operations.
    for (int v = 0; v < g.n; v++) {
        set<pair<int, int>> edges_unadded;
        set<pair<int, int>> edges_removed;
        set<int> set_v = sol.get_node_to_clusters(v);
        set<int> neighbourhood = neighbour_clusters(g, sol, v);
        for (int u : set_v) {
            if (sol.book.b_lp.co_occurence[minmax(v, u)] > 1) continue;
            if (g.has_edge(v, u)) edges_removed.insert(minmax(v, u));
            else edges_unadded.insert(minmax(v, u));
        }

        for (int si : neighbourhood) {
            int op_cost = 0;
            set<pair<int, int>> edges_readded;
            set<pair<int, int>> edge_remove_ignore;
            set<pair<int, int>> edges_added;
            set<pair<int, int>> edge_unadd_ignore;
            set<int> si_nodes = sol.get_set(si);
            for (int w : si_nodes) {
                if (sol.book.b_lp.co_occurence[minmax(v, w)] > 0) continue;
                if (g.has_edge(v, w)) {
                    if (edges_removed.find(minmax(v, w)) != edges_removed.end()) {
                        edge_remove_ignore.insert(minmax(v, w));
                    }
                    else {
                         edges_readded.insert(minmax(v, w));
                    }
                }
                else {
                    if (edges_unadded.find(minmax(v, w)) != edges_unadded.end()) {
                        edge_unadd_ignore.insert(minmax(v, w));
                    }
                    else {
                        edges_added.insert(minmax(v, w));
                    }
                }
            }
            vector<tuple<int, int, int>> to_change;

            for (pair<int,int> e : edges_readded) {}
        }

    }

}

optional<int> best_vertex_move(Graph &g, SolutionRepresentation &sol) {
    if (sol.book.b_lp.v == -1) precompute_b_lp(g, sol);
}