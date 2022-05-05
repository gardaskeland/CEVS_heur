#include "remove_add.h"

#define tri tuple<int, int, int>

struct Modifications {
    vector<pair<int, int>> nodes_removed_from_sets;
    set<int> neighbour_sets;
};

struct cmp_ascending {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.first < right.first;
    }
};

//Takes an additional argument with set mapped to nodes compared to removal_cost.
//Calculates as if this node is removed from the graph
int removal_cost_node_removed(Graph &g, SolutionRepresentation &sol, int si, int u, map<int, vector<int>> &changed) {
    set<int> &set_nodes = sol.clusters[si];
    //edges we must delete when deleting the node that are in G (+ to cost)
    int edges_to_delete = 0;
    //edges we must delete that when deleting the node that are not in G (- to cost)
    int edges_added = 0;
    bool in_same_cluster = false;
    bool other_condition = false;
    bool vertex_removed;
    if (changed.find(si) != changed.end()) {
        other_condition = true;
    }
    for (int i : set_nodes) {
        if (i == u) continue;
        if (other_condition) {
            vertex_removed = false;
            for (int v : changed[si]) {
                if (i == v) {
                    vertex_removed = true;
                    break;
                }
            }
            if (vertex_removed) continue;
        }
        if (sol.get_co_occurence(i, u) > 1) continue;

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

optional<int> remove_3(Graph &g, SolutionRepresentation &sol, tri &t, Modifications &mod) {
    int cost = 0;
    set<int> neighbours;
    vector<int> nodes = {get<0>(t), get<1>(t), get<2>(t)};
    for (int u : nodes) {
        for (int si : get_neighbour_set_of_u(g, sol, u)) neighbours.insert(si);
        for (int si : sol.node_in_clusters[u]) neighbours.insert(si);
    }
    
    mod.neighbour_sets = neighbours;

    vector<vector<pair<int, int>>> cost_and_set_to_remove_from(3, vector<pair<int, int>>());

    //Find first node to remove
    int best_index = -1;
    int best_cost = 1 << 15;
    for (int i = 0; i < 3; i++) {
        for (int si : sol.node_in_clusters[nodes[i]]) {
            cost_and_set_to_remove_from[i].emplace_back(make_pair(removal_cost(g, sol, si, nodes[i]), si));
        }
        if (cost_and_set_to_remove_from[i].empty()) continue;
        sort(cost_and_set_to_remove_from[i].begin(), cost_and_set_to_remove_from[i].end(), cmp_ascending());
        if (cost_and_set_to_remove_from[i][0].first <  best_cost) {
            best_cost = cost_and_set_to_remove_from[i][0].first;
            best_index = i;
        }
    }

    if (best_index == -1) return {};

    map<int, vector<int>> changed;
    int first_remove = nodes[best_index];
    changed[cost_and_set_to_remove_from[best_index][0].second] = {first_remove}; //do not do this the second time
    cost += cost_and_set_to_remove_from[best_index][0].first;
    mod.nodes_removed_from_sets.push_back(make_pair(nodes[best_index], cost_and_set_to_remove_from[best_index][0].second));
    cost_and_set_to_remove_from.erase(cost_and_set_to_remove_from.begin() + best_index);
    nodes.erase(nodes.begin() + best_index);

    //Find second node to remove
    best_cost = 1 << 15;
    best_index = -1;
    for (int i = 0; i < 2; i++) {
        if (sol.get_co_occurence(nodes[i], first_remove)) {
            cost_and_set_to_remove_from[i].clear();
            for (int si : sol.node_in_clusters[nodes[i]]) {
                cost_and_set_to_remove_from[i].emplace_back(make_pair(removal_cost_node_removed(g, sol, si, nodes[i], changed), si));
            }
        }
        if (cost_and_set_to_remove_from[i].empty()) continue;
        sort(cost_and_set_to_remove_from[i].begin(), cost_and_set_to_remove_from[i].end(), cmp_ascending());
        if (cost_and_set_to_remove_from[i][0].first <  best_cost) {
            best_cost = cost_and_set_to_remove_from[i][0].first;
            best_index = i;
        }
    }

    if (best_index == -1) return {};

    int second_remove = nodes[best_index];
    if (changed.find(cost_and_set_to_remove_from[best_index][0].second) != changed.end()) {
        changed[cost_and_set_to_remove_from[best_index][0].second].push_back(second_remove);
    } else {
        changed[cost_and_set_to_remove_from[best_index][0].second] = {second_remove};
    }
    cost += cost_and_set_to_remove_from[best_index][0].first;
    mod.nodes_removed_from_sets.push_back(make_pair(nodes[0], cost_and_set_to_remove_from[best_index][0].second));
    cost_and_set_to_remove_from.erase(cost_and_set_to_remove_from.begin() + best_index);
    nodes.erase(nodes.begin() + best_index);

    if (sol.get_co_occurence(nodes[0], second_remove)) {
        cost_and_set_to_remove_from[0].clear();
        for (int si : sol.node_in_clusters[nodes[0]]) {
            cost_and_set_to_remove_from[0].emplace_back(make_pair(removal_cost_node_removed(g, sol, si, nodes[0], changed), si));
        }
        sort(cost_and_set_to_remove_from[0].begin(), cost_and_set_to_remove_from[0].end(), cmp_ascending());
    }

    if (cost_and_set_to_remove_from[0].empty()) return {};

    cost += cost_and_set_to_remove_from[0][0].first;
    mod.nodes_removed_from_sets.push_back(make_pair(nodes[0], cost_and_set_to_remove_from[0][0].second));

    return cost;
}

//Removed contains node and set it is removed from
int add_node_to_set_cost_changed(Graph &g, SolutionRepresentation &sol, int si, int v, vector<pair<int, int>> removed, vector<pair<int, int>> added) {
    set<int> &set_nodes = sol.clusters[si];
    if (set_nodes.find(v) != set_nodes.end()) return 0;
    //edges between v and i deleted before adding the node (these edges are in G)
    int edges_to_delete = 0;
    //edges we must add after adding the node (these edges are not in G)
    int edges_to_add = 0;

    bool other_condition = false;
    for (pair<int, int> p : removed) {
        if (p.second == si) other_condition = true;
    }
    for (int i : set_nodes) {
        //cout << sol.get_co_occurence(i, v) << "\n";
        if (other_condition) {
            bool skip = false;
            for (pair<int, int> p : removed) {
                if (p.first == i && p.second == si) {
                    skip = true;
                    break;
                }
            }/**
            if (skip) {
                if (sol.get_co_occurence(i, v) == 1) {
                    if (g.has_edge(i, v)) edges_to_delete += g.get_edge_cost(i, v);
                    else edges_to_add += g.get_edge_cost(i, v);
                }
                continue;
           }*/
           continue;
        }

        if (sol.get_co_occurence(i, v) > 0) continue;

        if (g.has_edge(i, v)) {
            edges_to_delete += g.get_edge_cost(i, v);
        }
        
        //g does not have an edge between i and v
        else {
            edges_to_add += g.get_edge_cost(i, v);
        }
    }

    //Assume no pair is in both removed and added
    for (pair<int, int> p : added) {
        if (p.second == si && p.first != v) {
            if (sol.get_co_occurence(p.first, v) == 0) {
                 if (g.has_edge(p.first, v)) {
                    edges_to_delete += g.get_edge_cost(p.first, v);
                }
                else {
                    edges_to_add += g.get_edge_cost(p.first, v);
                }
            }
        }
    }
    //+1 since we split the node by adding it
    return edges_to_add - edges_to_delete + g.get_node_weight(v);
}

optional<int> add_3(Graph &g, SolutionRepresentation &sol, tri &t, Modifications &mod) {
    vector<vector<pair<int, int>>> cost_set(3, vector<pair<int, int>>());
    vector<int> nodes = {get<0>(t), get<1>(t), get<2>(t)};
    vector<pair<int, int>> removed = mod.nodes_removed_from_sets;
    vector<pair<int, int>> added;
    int cost = 0;

    int best_cost = 1 << 15;
    int best_index = -1;

    for (int i = 0; i < 3; i++) {
        for (int si : mod.neighbour_sets) {
            cost_set[i].emplace_back(make_pair(add_node_to_set_cost_changed(g, sol, si, nodes[i], removed, added), si));
        }
        if (cost_set[i].empty()) continue;
        sort(cost_set[i].begin(), cost_set[i].end(), cmp_ascending());
        if (cost_set[i][0].first < best_cost) {
            best_cost = cost_set[i][0].first;
            best_index = i;
        }
    }

    if (best_index == -1) return {};

    int first_add = nodes[best_index];
    cost += cost_set[best_index][0].first;
    pair<int, int> to_add = make_pair(first_add, cost_set[best_index][0].second);
    bool do_add = true;
    //Make sure there is no equal pair in added and removed to make add_node_to_set_unchanged work.
    for (auto it = removed.begin(); it != removed.end(); it++) {
        if (it->first == to_add.first && it->second == to_add.second) {
            removed.erase(it);
            do_add = false;
            break;
        }
    }
    if (do_add) {
        added.push_back(to_add);
    }
    cost_set.erase(cost_set.begin() + best_index);
    nodes.erase(nodes.begin() + best_index);

    best_cost = 1 << 15;
    best_index = -1;
    for (int i = 0; i < 2; i++) {
        cost_set[i].clear();
        for (int si : mod.neighbour_sets) {
            cost_set[i].emplace_back(make_pair(add_node_to_set_cost_changed(g, sol, si, nodes[i], removed, added), si));
        }
        if (cost_set[i].empty()) continue;
        sort(cost_set[i].begin(), cost_set[i].end(), cmp_ascending());
        if (cost_set[i][0].first < best_cost) {
            best_cost = cost_set[i][0].first;
            best_index = i;
        }
    }

    if (best_index == -1) return {};

    int second_add = nodes[best_index];
    cost += cost_set[best_index][0].first;
    to_add = make_pair(second_add, cost_set[best_index][0].second);
    do_add = true;
    //Make sure there is no equal pair in added and removed to make add_node_to_set_unchanged work.
    for (auto it = removed.begin(); it != removed.end(); it++) {
        if (it->first == to_add.first && it->second == to_add.second) {
            removed.erase(it);
            do_add = false;
            break;
        }
    }
    if (do_add) {
        added.push_back(to_add);
    }
    cost_set.erase(cost_set.begin() + best_index);
    nodes.erase(nodes.begin() + best_index);

    cost_set[0].clear();
    for (int si : mod.neighbour_sets) {
        cost_set[0].emplace_back(make_pair(add_node_to_set_cost_changed(g, sol, si, nodes[0], removed, added), si));
    }
    sort(cost_set[0].begin(), cost_set[0].end(), cmp_ascending());

    if (cost_set[0].empty()) return {};

    int third_add = nodes[0];
    cost += cost_set[0][0].first;
    to_add = make_pair(third_add, cost_set[0][0].second);

    do_add = true;
    for (auto it = removed.begin(); it != removed.end(); it++) {
        if (it->first == to_add.first && it->second == to_add.second) {
            removed.erase(it);
            do_add = false;
            break;
        }
    }
    if (do_add) {
        added.push_back(to_add);
    }

    sol.book.b_remove_add.next_move_remove = removed;
    sol.book.b_remove_add.next_move_add = added;

    return cost;
}

optional<int> remove_add_3(Graph &g, SolutionRepresentation &sol) {
    BRemoveAdd &b = sol.book.b_remove_add;
    //do precomputations for operator
    if (!b.initiated) {
        g.find_all_p3s();
        b.best_p3s = vector<pair<int, tri>>(g.all_p3s.size());
        b.initiated = true;
    }

    if (g.all_p3s.size() == 0) return {};

    if (b.counter == 0 || b.index >= b.best_p3s.size()) {
        int cost;
        int i = 0;
        Modifications mod;
        for (tri t : g.all_p3s) {
            optional<int> remove_cost = remove_3(g, sol, t, mod);
            if (!remove_cost.has_value()) return {};
            int cost = remove_cost.value();
            optional<int> add_cost = add_3(g, sol, t, mod);
            if (!add_cost.has_value()) return {};
            cost += add_cost.value();
            b.best_p3s[i++] = make_pair(cost, t);
        }
        b.counter = b.best_p3s.size() / 2;
        b.index = 0;
    }

    tri p3 = b.best_p3s[b.index++].second;
    Modifications mod;
    optional<int> remove_cost = remove_3(g, sol, p3, mod);
    if (!remove_cost.has_value()) return {};
    //cout << "remove_cost ok\n";
    //b.next move updated in add
    optional<int> add_cost = add_3(g, sol, p3, mod);
    if (!add_cost.has_value()) return {};
    //cout << "add_cost_ok\n";
    return remove_cost.value() + add_cost.value();
}   