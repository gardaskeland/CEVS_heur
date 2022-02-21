#include "add_node.h"

double relative_out_degree(Graph &g, SolutionRepresentation &sol, int si) {
    int count_out_edges = 0;
    set<int> nodes_in_si = sol.get_set(si);
    for (int node : nodes_in_si) {
        vector<int> neighbours = g.adj[node];
        for (int neighbour : neighbours) {
            if (nodes_in_si.find(neighbour) != nodes_in_si.end()) {
                continue;
            }
            else {
                count_out_edges += g.get_edge_cost(node, neighbour);
            }
        }
    }
    return count_out_edges / nodes_in_si.size();

}

void get_relative_out_degrees(Graph &g, SolutionRepresentation &sol) {
    set<int> indices;
    int temp;
    for (int i : sol.get_set_indices()) indices.insert(i);
    if (sol.book.b_add_node.last_add_operation == -1) {
        cout << "Precomputing relative_out_degrees\n";
        for (int i : sol.get_set_indices()) {
            temp = relative_out_degree(g, sol, i);
            sol.book.b_add_node.relative_out_degrees[i] = temp;
            sol.book.b_add_node.pq_relative_out_degrees.push(pair<int, int>(temp, i));
        }
    } else {
        set<int> modified_clusters = sol.book.modified_clusters.query(sol.book.b_add_node.last_add_operation, sol.book.operation_number - 1);
        for (int i : modified_clusters) {
            if (indices.find(i) != indices.end()) {
                temp = relative_out_degree(g, sol, i);
                sol.book.b_add_node.relative_out_degrees[i] = relative_out_degree(g, sol, i);
                sol.book.b_add_node.pq_relative_out_degrees.push(pair<int, int>(temp, i));
            }
        }   
    }
}

int highest_relative_out_degree(Graph &g, SolutionRepresentation &sol) {
    //map<double, int> relative_out_degrees = get_relative_out_degrees(g, sol);
    //map<double, int>::reverse_iterator it = relative_out_degrees.rbegin();
    //return it->second;
    get_relative_out_degrees(g, sol);
    pair<int, int> pending;
    set<int> indices;
    for (int i : sol.get_set_indices()) {
        indices.insert(i);
    }
    while (true) {
        pending = sol.book.b_add_node.pq_relative_out_degrees.top();
        if (indices.find(pending.second) != indices.end() && pending.first == sol.book.b_add_node.relative_out_degrees[pending.second]) {
            return pending.second;
        }
        sol.book.b_add_node.pq_relative_out_degrees.pop();
    }
    cout << "Error in highest_relative_out_degree\n";
    return -1;
}

int add_node_to_set_cost(Graph &g, SolutionRepresentation &sol, int si, int v) {
    set<int> set_nodes = sol.get_set(si);
    if (set_nodes.find(v) != set_nodes.end()) return 0;
    //edges between v and i deleted before adding the node (these edges are in G)
    int edges_to_delete = 0;
    //edges we must add after adding the node (these edges are not in G)
    int edges_to_add = 0;
    bool in_same_cluster = false;
    for (int i : set_nodes) {
        //cout << sol.get_co_occurence(i, v) << "\n";
        if (sol.get_co_occurence(i, v) > 0) continue;
        /**
        for (int w : sol.get_node_to_clusters(v)) {
            if (w == si) continue;
            set<int> w_cluster = sol.get_set(w);
            if (w_cluster.find(i) != w_cluster.end()) {
                in_same_cluster = true;
                break;
            }
        }
        */
        if (g.has_edge(i, v)) {
            edges_to_delete += g.get_edge_cost(i, v);
        }
        
        //g does not have an edge between i and v
        else {
            edges_to_add += g.get_edge_cost(i, v);
        }
    } 
    //+1 since we split the node by adding it
    return edges_to_add - edges_to_delete + g.get_node_weight(v);
}


vector<pair<int, int>> best_nodes_to_add(Graph &g, SolutionRepresentation &sol, int si) {
    vector<pair<int, int>> results;
    set<int> marked_neighbours;
    set<int> set_nodes = sol.get_set(si);
    for (int v : set_nodes) {
        for (int w : g.adj[v]) {
            if (set_nodes.find(w) != set_nodes.end() || marked_neighbours.find(w) != marked_neighbours.end()) {
                continue;
            } else {
                results.push_back(pair<int, int>(add_node_to_set_cost(g, sol, si, w), w));
                marked_neighbours.insert(w);
            }
        }
    }
    sort(results.begin(), results.end());
    return results;
}


struct revert_add_node {
    int v;
    int si;
};


void do_revert_add_node(SolutionRepresentation &sol, Bookkeep &book) {
    sol.remove(book.revert_add_node[0], book.revert_add_node[1]);
}


optional<int> random_choice_add_node(Graph &g, SolutionRepresentation &sol) {
    vector<int> set_indices = sol.get_set_indices();
    int si = set_indices[rand() % set_indices.size()];
    vector<pair<int, int>> best_nodes = best_nodes_to_add(g, sol, si);
    if (best_nodes.size() == 0) {
        sol.book.b_add_node.v = -1;
        return {};
    }
    sol.book.b_add_node.v = best_nodes[0].second;
    sol.book.b_add_node.si = si;
    return optional(best_nodes[0].first);
}

int add_node_to_all(Graph &g, SolutionRepresentation &sol) {
    int cost = 0;
    vector<int> set_indices = sol.get_set_indices();
    for (int si : set_indices) {
        vector<pair<int, int>> best_nodes = best_nodes_to_add(g, sol, si);
        if (best_nodes.size() == 0) {
            //sol.book.b_add_node.v = -1;
            continue;
        }
        //sol.book.b_add_node.v = best_nodes[0].second;
        //sol.book.b_add_node.si = si;
        sol.add(best_nodes[0].second, si);
        cost += best_nodes[0].first;
    }
    return cost;
}

int add_node_to_neighbours(Graph &g, SolutionRepresentation &sol, int u) {
    int cost = 0;
    int add_cost;

    set<int> neighbours;
    set<int> current;
    for (int v : g.adj[u]) {
        for (int s : sol.get_node_to_clusters(v)) {
            current = sol.get_set(s);
            if (current.find(u) != current.end()) continue;
            neighbours.insert(s);
        }
    }

    for (int s : neighbours) {
        add_cost = add_node_to_set_cost(g, sol, s, u);
        if (add_cost <= 0) {
            sol.add(u, s);
            cost += add_cost;
        }
    }

    return cost;
}

int add_all_nodes_to_neighbours(Graph &g, SolutionRepresentation &sol) {
    int cost = 0;
    for (int u = 0; u < g.n; u++) {
        cost += add_node_to_neighbours(g, sol, u);
    }
    return cost;
}


int add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book) {
    int si = highest_relative_out_degree(g, sol);
    vector<pair<int, int>> best_nodes = best_nodes_to_add(g, sol, si);
    if (best_nodes.size() == 0) return 0;
    sol.book.b_add_node.v = best_nodes[0].second;
    sol.book.b_add_node.si = si;
    //sol.add(best_nodes.begin()->second, si);
    //book.revert_add_node[0] = best_nodes.begin()->second;
    //book.revert_add_node[1] = si;
    //sol.print_solution();
    return best_nodes[0].first;
}

int removal_cost(Graph &g, SolutionRepresentation &sol, int si, int u) {
    set<int> set_nodes = sol.get_set(si);
    //edges we must delete when deleting the node that are in G (+ to cost)
    int edges_to_delete = 0;
    //edges we must delete that when deleting the node that are not in G (- to cost)
    int edges_added = 0;
    bool in_same_cluster = false;
    for (int i : set_nodes) {
        if (i == u) continue;
        if (sol.get_co_occurence(i, u) > 1) continue;
        /**
        in_same_cluster = false;
        for (int w : sol.get_node_to_clusters(u)) {
            if (w == si) continue;
            set<int> w_cluster = sol.get_set(w);
            if (w_cluster.find(i) != w_cluster.end()) {
                in_same_cluster = true;
                break;
            }
        }
        */
        if (g.has_edge(i, u)) {
            edges_to_delete += g.get_edge_cost(i, u);
        }
        
        //g does not have an edge between i and v
        else {
            edges_added += g.get_edge_cost(i, u);
        }
    }
    //-1 since we split one less
    return edges_to_delete - edges_added - g.get_node_weight(u);
}


//Problem: Removing a node from a cluster can affect the cost of removing the node from
//other clusters if they share common vertices. Can therefore only remove the one with best cost
//at all times, before calculating all over. May be very expensive, worst case O(n^3 * m) at least.
int remove_nodes_(Graph &g, SolutionRepresentation &sol) {
    int cost = 0;
    for (int u = 0; u < g.n; u++) {
        while (true) {
            set<int> in_clusters = sol.get_node_to_clusters(u);
            if (in_clusters.size() <= 1) break;
            int least_cost = pow(2, 30);
            int best_set = -1;
            for (int s : in_clusters) {
                //if (sol.get_set(s).size() == 1) continue;
                //cout << "least cost: " << least_cost << "\n";
                int cost = removal_cost(g, sol, s, u);
                if (cost <= min(0, least_cost)) {
                    least_cost = cost;
                    best_set = s;
                }  
            }
            if (best_set == -1) break;
            cost += least_cost;
            sol.remove(u, best_set);
        }
    }
    return cost;
}


void do_add(SolutionRepresentation &sol) {
    sol.add(sol.book.b_add_node.v, sol.book.b_add_node.si);
}
        /**
        set<int> in_clusters = sol.get_node_to_clusters(u);
        //True if some vertex has cost at least 0, so that at least one copy of the vertex remains.
        bool can_remove = false;
        //Maps cost of removal to cluster index
        vector<pair<int, int>> clusters_to_remove_from;
        for (int s : in_clusters) {
            int cost = removal_cost(g, sol, u, s);
            if (cost >= 0) {
                can_remove = true;
            } else {
                clusters_to_remove_from.push_back(pair<int, int>(cost, s));
            }
        }
        if (can_remove) {
            for (pair<int, int> p : clusters_to_remove_from) {
                if (p.first < 0) sol.remove(u, p.second);
            }
        } else {
            sort(clusters_to_remove_from.begin(), clusters_to_remove_from.end());
            for (int i = 0; i < clusters_to_remove_from.size() - 1; i++) {
                //if (clusters_to_remove_from[i].first >= 0) break; all first must be less than zero.
                sol.remove(u, clusters_to_remove_from[i].second);
            }
        }
    }
}

/*
//Possibly not working yet.
int weighted_random_add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book) {
    map<double, int> out_deg = get_relative_out_degrees(g, sol);
    int ind = weighted_random_index(10, out_deg.size(), 1.7);
    
    int counter = 0;
    map<double, int>::iterator it = out_deg.begin();
    while (counter != ind) {
        it++;
        counter += 1;
    }
    int si = it->second;

    map<int, int> best_nodes = best_nodes_to_add(g, sol, si);
    ind = weighted_random_index(2, out_deg.size(), 3);

    counter = 0;
    map<int, int>::iterator it2 = best_nodes.begin();
    while (counter != ind) {
        it2++;
        counter += 1;
    }

    sol.book.b_add_node.v = it->second;
    sol.book.b_add_node.si = si;

    return it->first;
    //book.revert_add_node[0] = it2->second;
    //book.revert_add_node[1] = si;
}
**/