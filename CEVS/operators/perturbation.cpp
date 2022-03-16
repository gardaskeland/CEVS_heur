#include "perturbation.h"

//returns -1 for empty set
int choose_random_set_entry(set<int> s) {
    if (s.size() == 0) {
        return -1;
    }
    int r = rand() % s.size();
    int index = 0;
    //Think this is the quickest way when we store the solution with sets.
    int si = -1;
    for (int elm : s) {
        if (index == r) {
            si = elm;
            break;
        }
        index += 1;
    }
    if (si == -1) {
        cout << "Error in choose_random_set_entry";
        return -1;
    }
    return si;
}

int vertex_isolator(Graph &g, SolutionRepresentation &sol) {
    int v = rand() % g.n;
    //sol.print_solution();
    set<int> sets = sol.get_node_to_clusters(v);
    int si = choose_random_set_entry(sets);
    int cost = removal_cost(g, sol, si, v);
    sol.remove(v, si);
    set<int> to_add;
    to_add.insert(v);
    sol.add_set(to_add);
    //sol.print_solution();
    //cout << "node weight: " << g.get_node_weight(v) << "\n";
    return cost + g.get_node_weight(v);
}

int vertex_mover(Graph &g, SolutionRepresentation &sol) {
    int v = rand() % g.n;
    //never empty
    set<int> sets = sol.get_node_to_clusters(v);
    int from = choose_random_set_entry(sets);
    //may be empty
    set<int> adjacent_clusters = neighbour_clusters(g, sol, v);
    if (adjacent_clusters.size() == 0) return 0;
    int to = choose_random_set_entry(adjacent_clusters);
    int cost = removal_cost(g, sol, from, v);
    sol.remove(v, from);
    cost += add_node_to_set_cost(g, sol, to, v);
    sol.add(v, to);
    return cost;
}

bool in_other_cluster(Graph &g, SolutionRepresentation &sol, int si, int u, int v) {
    set<int> u_clusters = sol.get_node_to_clusters(u);
    for (int sj : sol.get_node_to_clusters(v)) {
        if (si == sj) continue;
        if (u_clusters.find(sj) != u_clusters.end()) return true;
    }
    return false;
}

int clique_splitter(Graph &g, SolutionRepresentation &sol) {
    int v = rand() % g.n;
    set<int> sets = sol.get_node_to_clusters(v);
    int from = choose_random_set_entry(sets);
    set<int> set_with_v = sol.get_set(from);
    set<int> u_alternatives;
    for (int u : g.adj[v]) {
        if (set_with_v.find(u) != set_with_v.end()) u_alternatives.insert(u);
    }
    if (u_alternatives.size() == 0) return 0;

    int u = choose_random_set_entry(u_alternatives);
    //check if they are both in some other cluster as well
    bool in_cluster = in_other_cluster(g, sol, from, u, v);
    int cost = removal_cost(g, sol, from, v);
    sol.remove(v, from);
    cost += removal_cost(g, sol, from, u);
    sol.remove(u, from);
    set<int> new_set; new_set.insert(u); new_set.insert(v);
    sol.add_set(new_set);
    if (in_other_cluster) {
        return cost + g.get_node_weight(u) + g.get_node_weight(v);
    } else {
        return cost + g.get_node_weight(u) + g.get_node_weight(v) - g.get_edge_cost(u, v);
    }
}

struct cmp_descending {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.first > right.first;
    }
};

struct cmp_ascending {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.first < right.first;
    }
};

int find_cost_add_trio(Graph &g, SolutionRepresentation &sol, int u, int v, int w) {
    int cost = g.get_node_weight(u) + g.get_node_weight(v) + g.get_node_weight(w);
    if (sol.get_co_occurence(u, v) == 0) {
        if (g.has_edge(u, v)) cost -= g.get_edge_cost(u, v);
        else cost += g.get_edge_cost(u, v);
    }
    if (sol.get_co_occurence(u, w) == 0) {
        if (g.has_edge(u, w)) cost -= g.get_edge_cost(u, w);
        else cost += g.get_edge_cost(u, w);
    }
    if (sol.get_co_occurence(v, w) == 0) {
        if (g.has_edge(v, w)) cost -= g.get_edge_cost(v, w);
        else cost += g.get_edge_cost(v, w);
    } 
    return cost;
}

optional<int> add_trio(Graph &g, SolutionRepresentation &sol) {
    vector<pair<int, tri>> candidates;
    vector<pair<int, int>> inv_coveredness;
    int covered;
    //pick a vertex
    for (int v = 0; v < g.n; v++) {
        covered = 0;
        for (int w : g.adj[v]) {
            if (sol.get_co_occurence(v, w) == 0) covered += 1;
        }
    inv_coveredness.emplace_back(make_pair(g.adj[v].size() - covered, v));
    }
    sort(inv_coveredness.begin(), inv_coveredness.end(), cmp_descending());

    int k = weighted_random_index(20, inv_coveredness.size(), 2);
    pair<int, int> p = inv_coveredness[k];

    int cost;
    int best_cost = pow(2, 16) - 1;
    tri best_nodes = tri(-1, -1, -1);
    vector<int> &s = g.adj[p.second];
    for (int i = 0; i < s.size() - 1; i++) {
        for (int j = i + 1; j < s.size(); j++) {
            cost = find_cost_add_trio(g, sol, p.second, s[i], s[j]);
            if (cost < best_cost) {
                best_cost = cost;
                best_nodes = tri(p.second, s[i], s[j]);
            }
        }
    }
    if (get<0>(best_nodes) == -1) return {};
    sol.book.b_perturbation.to_add = best_nodes;
    return optional<int>(best_cost);
}

int remove_set_cost(Graph &g, SolutionRepresentation &sol, int si) {
    vector<int> si_nodes;
    for (int u : sol.clusters[si]) {
        si_nodes.push_back(u);
    }
    int cost = 0;
    for (int i = 0; i < si_nodes.size() - 1; i++) {
        for (int j = i+1; j < si_nodes.size(); j++) {
            if (sol.get_co_occurence(si_nodes[i], si_nodes[j]) == 1) {
                if (g.has_edge(si_nodes[i], si_nodes[j])) cost += g.get_edge_cost(si_nodes[i], si_nodes[j]);
                else cost -= g.get_edge_cost(si_nodes[i], si_nodes[j]);
            }
        }
    }
    for (int u : si_nodes) {
        cost -= g.get_node_weight(u);
    }
    return cost;
}

optional<int> remove_set_op(Graph &g, SolutionRepresentation &sol) {
    vector<pair<int, int>> best_to_remove;
    bool can_remove;
    for (int si : sol.get_set_indices()) {
        //indicates we can remove the set without getting an unfeasible solution
        can_remove = true;
        for (int u : sol.clusters[si]) {
            if (sol.node_in_clusters[u].size() == 1) {
                can_remove = false;
                // cout << "Cannot remove!\n";
                break;
            }
        }
        if (!can_remove) continue;
        best_to_remove.emplace_back(make_pair(remove_set_cost(g, sol, si), si));
    }
    if (best_to_remove.empty()) return {};
    sort(best_to_remove.begin(), best_to_remove.end(), cmp_ascending());
    if (best_to_remove[0].first >= 0) return {};

    //for (pair<int, int> p : best_to_remove) {
     //  cout << p.first << " ";
    //}
    //cout << "\n";

    int k = 0;
    pair<int, int> to_remove = best_to_remove[k];
    sol.book.b_perturbation.si_to_remove = to_remove.second;
    return optional<int>(to_remove.first);
}

//Assumes u and v is not in the same set.
int swap_cost(Graph &g, SolutionRepresentation &sol, int u, int v, int si, int sj) {
    int cost = cost_of_move(g, sol, u, si, sj) + cost_of_move(g, sol, v, sj, si);
    //removing cost counted from add
    if (g.has_edge(u, v)) {
        cost += 2 * g.get_edge_cost(u, v);
    } else {
        cost -= 2 * g.get_edge_cost(u, v);
    }
    return cost;
}

struct cmp_swap {
    bool operator() (tuple<int, pair<int, int>, pair<int, int>> &left, tuple<int, pair<int, int>, pair<int, int>> &right) {
        return get<0>(left) < get<0>(right);
    }
};

optional<int> swap(Graph &g, SolutionRepresentation &sol) {
    //cost, node1, node2
    vector<tuple<int, pair<int, int>, pair<int, int>>> candidates;
    int cost;
    int u = rand() % g.n;
    for (int v = 0; v < g.n; v++) {
        if (u == v || sol.get_co_occurence(u, v) > 0) continue;
        for (int si : sol.get_node_to_clusters(u)) {
            for (int sj : sol.get_node_to_clusters(v)) {
                cost = swap_cost(g, sol, u, v, si, sj);
                candidates.emplace_back(tuple<int, pair<int, int>, pair<int, int>>(cost, make_pair(u, v), make_pair(si, sj)));
            }
        }
    }
    if (candidates.empty()) return {};
    sort(candidates.begin(), candidates.end(), cmp_swap());
    long k = weighted_random_index(20, candidates.size(), 2);
    tuple<int, pair<int, int>, pair<int, int>> choice = candidates[k];
    sol.book.b_perturbation.to_swap = get<1>(choice);
    sol.book.b_perturbation.sets_to_swap = get<2>(choice);
    return optional<int>(get<0>(choice));
}

struct cmp_int_set {
    bool operator() (pair<int, set<int>> &left, pair<int, set<int>> &right) {
        return left.first < right.first;
    }
};

optional<int> add_set_over_uncovered(Graph &g, SolutionRepresentation &sol) {
    int covered;
    vector<pair<int, int>> best;
    for (int u = 0; u < g.n - 1; u++) {
        covered = 0;
        for (int v : g.adj[u]) {
            if (sol.get_co_occurence(u, v) > 0) covered += 1;
        }
        best.emplace_back(pair<int, int>(g.adj[u].size() - covered, u));
    }
    if (best.empty()) return {};

    sort(best.begin(), best.end(), cmp_descending());
    int sz = best.size();

    vector<pair<int, set<int>>> candidates;

    for (int i = 0; i < min(10, sz); i++) {

        int center_node = best[i].second;
        vector<int> to_add;
        for (int v : g.adj[center_node]) {
            if (sol.get_co_occurence(center_node, v) == 0 && g.has_edge(center_node, v)) to_add.push_back(v);
        }
        to_add.push_back(center_node);

        int cost = 0;
        /**
        cout << "Center node is " << center_node << "\n";
        cout << "to_add is ";
        for (int x : to_add) cout << x << " ";
        cout << "\n";*/
        for (int i = 0; i < to_add.size() - 1; i++) {
            for (int j = i + 1; j < to_add.size(); j++) {
                if (sol.get_co_occurence(to_add[i], to_add[j]) == 0) {
                    //cout << to_add[i] << " and " << to_add[j] << " not co-occuring. Has edge: " << g.has_edge(to_add[i], to_add[j]) << "\n";
                    if (g.has_edge(to_add[i], to_add[j])) cost -= g.get_edge_cost(to_add[i], to_add[j]);
                    else cost += g.get_edge_cost(to_add[i], to_add[j]);
                }
            }
        }

        set<int> to_add_set;
        for (int x : to_add) {
            cost += g.get_node_weight(x);
            to_add_set.insert(x);
        }

        //cout << "Cost of adding: " << cost << "\n";

        candidates.emplace_back(make_pair(cost, to_add_set));
    }

    sort(candidates.begin(), candidates.end(), cmp_int_set());

    //for (int i = 0; i < candidates.size(); i++) {
    //    cout << candidates[i].first << " ";
    //} cout << "\n";

    int k = weighted_random_index(20, candidates.size(), 2);

    sol.book.b_perturbation.set_to_add = candidates[k].second;
    return optional<int>(candidates[k].first);
}

int add_adjacent_vertex(Graph &g, SolutionRepresentation &sol) {
    set<int> adjacent_vertices;
    vector<int> indices = sol.get_set_indices();
    int si = indices[rand() % indices.size()];
    set<int> set_si = sol.get_set(si);
    for (int u : set_si) {
        for (int v : g.adj[u]) {
            if (!(set_si.find(v) != set_si.end())) {
                adjacent_vertices.insert(v);
            }
        }
    }
    if (adjacent_vertices.size() == 0) return 0;
    int to_add = choose_random_set_entry(adjacent_vertices);
    int cost = add_node_to_set_cost(g, sol, si, to_add);
    sol.add(to_add, si);
    return cost;
}