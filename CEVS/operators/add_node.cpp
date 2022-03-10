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

/**
 * @brief Modified to take a set seen which works a co-occurence for node v and some other node.
 * Specifically, if a node u is in seen, we calculate with u and v being in the same set. Useful
 * for when we want to add the same node to several sets.
 * 
 * @param g 
 * @param sol 
 * @param si 
 * @param v 
 * @param seen 
 * @return int 
 */
int add_node_to_set_cost_seen(Graph &g, SolutionRepresentation &sol, int si, int v, set<int> seen) {
    set<int> set_nodes = sol.get_set(si);
    if (set_nodes.find(v) != set_nodes.end()) return 0;
    //edges between v and i deleted before adding the node (these edges are in G)
    int edges_to_delete = 0;
    //edges we must add after adding the node (these edges are not in G)
    int edges_to_add = 0;
    bool in_same_cluster = false;
    for (int i : set_nodes) {
        //cout << sol.get_co_occurence(i, v) << "\n";
        if (sol.get_co_occurence(i, v) > 0 || seen.find(i) != seen.end()) continue;
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

struct cmp_descending {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.first < right.first;
    }
};

struct cmp_tri_descending {
    bool operator() (tri &left, tri &right) {
        return get<0>(left) < get<0>(right);
    }
};

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

optional<int> add_node_to_set(Graph &g, SolutionRepresentation &sol) {
    BAddNode &b = sol.book.b_add_node;
    if (sol.num_sets() == 0) return optional<int>();
    else if (b.add_node_to_set_counter == 0 || b.best_nodes_to_add_to_set.empty()) {
        b.best_nodes_to_add_to_set.clear(); //remember to do this for other ops as well.
        int best_node = -1;
        int best_set = -1;
        int best_cost = pow(2, 16) - 1;
        int cost;
        set<int> sets = sol.get_set_indices_as_set();
        set<int> neighbours;
        set<int> si_nodes;
        for (int si : sets) {
            neighbours.clear();
            si_nodes = sol.get_set(si);
            for (int u : si_nodes) {
                for (int v : g.adj[u]) {
                    if (si_nodes.find(v) != si_nodes.end()) continue;
                    neighbours.insert(v);
                }
            }
            for (int v : neighbours) {
                cost = add_node_to_set_cost(g, sol, si, v);
                if (cost < best_cost) {
                    best_cost = cost;
                    best_set = si;
                    best_node = v;
                }
                b.best_nodes_to_add_to_set.emplace_back(tri(cost, v, si));
            }
        }
        sort(b.best_nodes_to_add_to_set.begin(), b.best_nodes_to_add_to_set.end(), cmp_tri_descending());
        b.best_nodes_to_add_to_set.pop_back();
        if (best_node == -1) return optional<int>();
        b.add_node_to_set_counter = g.n / 10;
        b.v = best_node;
        b.si = best_set;
        return optional<int>(best_cost);
    } else {
        //cost, node, set
        b.add_node_to_set_counter--;
        tri best = b.best_nodes_to_add_to_set.back();
        b.best_nodes_to_add_to_set.pop_back();

        set<int> sets = sol.get_set_indices_as_set();
        if (sets.find(get<2>(best)) != sets.end()) return add_node_to_set(g, sol);
        
        set<int> si_nodes = sol.get_set(get<2>(best));
        if (si_nodes.find(get<1>(best)) != si_nodes.end()) return add_node_to_set(g, sol);
        
        int cost = add_node_to_set_cost(g, sol, get<2>(best), get<1>(best));
        b.v = get<1>(best);
        b.si = get<2>(best);
        return optional<int>(cost);
    }
}

set<int> get_neighbour_set_of_u(Graph &g, SolutionRepresentation &sol, int u) {
    set<int> current;
    set<int> neighbours;
    for (int v : g.adj[u]) {
        for (int s : sol.get_node_to_clusters(v)) {
            current = sol.get_set(s);
            if (current.find(u) != current.end()) continue;
            neighbours.insert(s);
        }
    }
    return neighbours;
}

int add_node_to_neighbours(Graph &g, SolutionRepresentation &sol, int u) {
    int cost = 0;
    int add_cost;

    set<int> neighbours = get_neighbour_set_of_u(g, sol, u);
    
    for (int s : neighbours) {
        add_cost = add_node_to_set_cost(g, sol, s, u);
        if (add_cost <= 0) {
            sol.add(u, s);
            cost += add_cost;
        }
    }

    return cost;
}

tuple<int, vector<int>> add_node_to_all_neighbours_accept(Graph &g, SolutionRepresentation &sol, int u) {
    int cost = 0;
    int add_cost;
    vector<int> to_add_to;
    set<int> neighbour_sets = get_neighbour_set_of_u(g, sol, u);
    set<int> seen;
    for (int s : neighbour_sets) {
        add_cost = add_node_to_set_cost_seen(g, sol, s, u, seen);
        if (add_cost <= 0) {
            cost += add_cost;
            to_add_to.push_back(s);
            for (int i : sol.get_set(s)) {
                seen.insert(i);
            }
        }
    }
    return tuple<int, vector<int>>(cost, to_add_to);
}

optional<int> add_node_to_neighbours_accept(Graph &g, SolutionRepresentation &sol) {
    if (sol.num_sets() == 1) {
        return optional<int>();
    }
    if (sol.book.b_add_node.add_node_counter == 0 || sol.book.b_add_node.best_vertices_to_add.empty()) {
        sol.book.b_add_node.best_vertices_to_add.clear();
        int best_node = -1;
        int best_cost = pow(2, 16) - 1;
        vector<int> sets_to_add_to;
        tuple<int, vector<int>> result;
        for (int u = 0; u < g.n; u++) {
            result = add_node_to_all_neighbours_accept(g, sol, u);
            if (get<1>(result).size() == 0) continue;
            if (get<0>(result) < best_cost) {
                best_cost = get<0>(result);
                best_node = u;
                sets_to_add_to = get<1>(result);
            }
            sol.book.b_add_node.best_vertices_to_add.emplace_back(pair<int, int>(get<0>(result), u));
        }
        if (best_node == -1) return optional<int>();

        sort(sol.book.b_add_node.best_vertices_to_add.begin(), 
            sol.book.b_add_node.best_vertices_to_add.end(), cmp_descending());
        sol.book.b_add_node.best_vertices_to_add.pop_back();

        sol.book.b_add_node.add_node_counter = g.n / 10;
        sol.book.b_add_node.v = best_node;
        sol.book.b_add_node.sets_to_change = sets_to_add_to;

        return optional<int>(best_cost);
    }
    else {
        sol.book.b_add_node.add_node_counter--;
        pair<int, int> best = sol.book.b_add_node.best_vertices_to_add.back();
        sol.book.b_add_node.best_vertices_to_add.pop_back();
        tuple<int, vector<int>> result = add_node_to_all_neighbours_accept(g, sol, best.second);
        sol.book.b_add_node.v = best.second;
        sol.book.b_add_node.sets_to_change = get<1>(result);
        return optional<int>(get<0>(result)); 
    }
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

int removal_cost_seen(Graph &g, SolutionRepresentation &sol, int si, int u, vector<int> &seen) {
    set<int> set_nodes = sol.get_set(si);
    //edges we must delete when deleting the node that are in G (+ to cost)
    int edges_to_delete = 0;
    //edges we must delete that when deleting the node that are not in G (- to cost)
    int edges_added = 0;
    bool in_same_cluster = false;
    for (int i : set_nodes) {
        if (i == u) continue;
        if (sol.get_co_occurence(i, u) - seen[i] != 1) continue;
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

//returns cost and vertices to remove
pair<int, vector<int>> cost_of_remove_node(Graph &g, SolutionRepresentation &sol, int u) {
    int total_cost = 0;
    int cost;
    set<int> u_sets = sol.get_node_to_clusters(u);
    vector<int> seen(g.n, 0);
    vector<int> to_remove;
    for (int si : u_sets) {
        if (to_remove.size() == u_sets.size() - 1) break; 
        cost = removal_cost_seen(g, sol, si, u, seen);
        if (cost <= 0) {
            to_remove.push_back(si);
            total_cost += cost;
            for (int v : sol.get_set(si)) {
                if (u == v) continue;
                seen[v] += 1;
            }
        }
    }
    return make_pair(total_cost, to_remove);
}

optional<int> remove_node_accept(Graph &g, SolutionRepresentation &sol) {
    BAddNode &b = sol.book.b_add_node;
    if (sol.num_sets() == 1) return optional<int>();
    if (b.remove_node_counter == 0 || b.best_nodes_to_remove.empty()) {
        b.best_nodes_to_remove.clear();
        int best_node = -1;
        vector<int> sets_to_remove_from;
        int best_cost = pow(2, 16) - 1;
        pair<int, vector<int>> result;
        for (int u = 0; u < g.n; u++) {
            result = cost_of_remove_node(g, sol, u);
            if (result.second.empty()) continue;
            if (result.first < best_cost) {
                best_cost = result.first;
                sets_to_remove_from = result.second;
                best_node = u;
            }
            b.best_nodes_to_remove.emplace_back(make_pair(result.first, u));
        }
        if (best_node == -1) return optional<int>();
        sort(b.best_nodes_to_remove.begin(), b.best_nodes_to_remove.end(), cmp_descending());
        b.best_nodes_to_remove.pop_back();
        b.remove_node_counter = g.n / 10;
        b.sets_to_change = sets_to_remove_from;
        b.v = best_node;
        return optional<int>(best_cost);
    }
    else {
        b.remove_node_counter--;
        int u = b.best_nodes_to_remove.back().second;
        b.best_nodes_to_remove.pop_back();
        pair<int, vector<int>> result = cost_of_remove_node(g, sol, u);
        if (result.second.empty()) return optional<int>();
        b.remove_node_counter = g.n / 10;
        b.sets_to_change = result.second;
        b.v = u;
        return optional<int>(result.first);
    }
        


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