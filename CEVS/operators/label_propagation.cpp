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

//May use in ALNS, but is always executed since we do it for all vertices and it is a
//local search heuristic.
int label_propagation_round(Graph &g, SolutionRepresentation &sol) {
    int cost = 0;
    int original_cost = sol.cost_solution(g);
    vector<int> to_permute = sol.to_permute;
    int sol_diff = 0;
    //Shuffle from: https://en.cppreference.com/w/cpp/algorithm/random_shuffle 13.01.2021
    std::random_device rd;
    std::mt19937 mt(rd());
    shuffle(to_permute.begin(), to_permute.end(), mt);
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
        //This one may not always give the correct cost...
        //cout << "best_set_cost: " << best_set_cost << "\n";
        if ((best_set_cost > 0 && vertex_cost + g.get_node_weight(u) <= 0) || best_set_add == -1) {
            set<int> to_add;
            to_add.insert(u);
            sol.add_set(to_add);
            //0 for adding, all cost measured in removal.
            //+1 since we add back a node.
            cost += vertex_cost + g.get_node_weight(u);
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
    }
    return cost;
}

int cost_of_move(Graph &g, SolutionRepresentation &sol, int u, int si, int sj) {
    int cost = 0;
    set<int> si_nodes = sol.get_set(si);
    set<int> sj_nodes = sol.get_set(sj);

    for (int v : si_nodes) {
        if (u == v) continue;
        if (sol.get_co_occurence(u, v) >= 2 || sj_nodes.find(v) != sj_nodes.end()) continue;

        if (g.has_edge(u, v)) cost += g.get_edge_cost(u, v);
        else cost -= g.get_edge_cost(u, v);
    }

    for (int v : sj_nodes) {
        //If u and v is in si, co_occurence > 0.
        if (sol.get_co_occurence(u, v) > 0) continue;

        if (g.has_edge(u, v)) cost -= g.get_edge_cost(u, v);
        else cost += g.get_edge_cost(u, v);
    }
    return cost;
}

optional<tri> find_best_move(Graph &g, SolutionRepresentation &sol, int u) {
    set<int> neighbour_sets = neighbour_clusters(g, sol, u);
    if (neighbour_sets.size() == 0) return optional<tri>();

    int cost;
    int best_cost = pow(2, 16) - 1;
    tri best_move = tri(-1, -1, 0);

    for (int si: sol.get_node_to_clusters(u)) {
        for (int sj : neighbour_sets) {
            cost = cost_of_move(g, sol, u, si, sj);
            if (cost < best_cost) {
                best_cost = cost;
                best_move = tri(si, sj, cost);
            }
        }
        cost = removal_cost(g, sol, si, u) + g.get_node_weight(u);
        if (cost < best_cost) {
            best_cost = cost;
            best_move = tri(si, -1, cost);
        }
        //cout << "best to move " << u << " from set " << si << " to set " << get<1>(best_move) << " with cost " << get<2>(best_move) << "\n"; 
    }

    if (get<0>(best_move) == -1) {
        return optional<tri>();
    } else {
        return optional<tri>(best_move);
    }
}

struct cmp_tri_ascending {
    bool operator() (tri &left, tri &right) {
        return get<0>(left) < get<0>(right);
    }
};

//Like find_best_move, but returns several moves
vector<tri> find_best_moves(Graph &g, SolutionRepresentation &sol, int u) {
    set<int> neighbour_sets = neighbour_clusters(g, sol, u);
    vector<tri> good_moves;
    if (neighbour_sets.size() == 0) return vector<tri>();

    int cost;
    int best_cost = pow(2, 16) - 1;
    tri best_move = tri(-1, -1, 0);

    for (int si: sol.get_node_to_clusters(u)) {
        for (int sj : neighbour_sets) {
            cost = cost_of_move(g, sol, u, si, sj);
            good_moves.emplace_back(tri(cost, si, sj));
        }
        cost = removal_cost(g, sol, si, u) + g.get_node_weight(u);
        good_moves.emplace_back(tri(cost, si, -1));
        //cout << "best to move " << u << " from set " << si << " to set " << get<1>(best_move) << " with cost " << get<2>(best_move) << "\n"; 
    }

    sort(good_moves.begin(), good_moves.end(), cmp_tri_ascending());

    return good_moves;
}

struct cmp_greater_pair {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.first > right.first;
    }
};

optional<int> label_propagation_accept(Graph &g, SolutionRepresentation &sol) {
    optional<tri> temp;
    //map<int, tri> best;
    if (sol.num_sets() < 2) {
        return optional<int>();
    }
    else if (sol.book.b_lp.label_prop_counter == 0 || sol.book.b_lp.best_label_prop.empty()) {
        sol.book.b_lp.best_label_prop.clear();
        //set to move from and to, cost
        
        for (int v = 0; v < g.n; v++) {
            temp = find_best_move(g, sol, v);
            if (!temp.has_value()) {
                continue;
            }
            //cout << "get 2 of temp: :" << get<2>(temp.value()) << ":\n";
            sol.book.b_lp.best_label_prop.emplace_back(pair<int, int>(get<2>(temp.value()), v));
            //best[v] = temp.value();
        }
        if (sol.book.b_lp.best_label_prop.empty()) {
            return optional<int>();
        }
        sort(sol.book.b_lp.best_label_prop.begin(), sol.book.b_lp.best_label_prop.end(), cmp_greater_pair());

        //sol.book.b_lp.best_label_prop.pop_back();
        sol.book.b_lp.label_prop_counter = g.n / 2;
    } else {
        sol.book.b_lp.label_prop_counter--;
    }

    pair<int, int> op = sol.book.b_lp.best_label_prop.back();
    sol.book.b_lp.best_label_prop.pop_back();

    temp = find_best_move(g, sol, op.second);
    if (!temp.has_value()) return {};
    sol.book.b_lp.next_move = tri(op.second, get<0>(temp.value()), get<1>(temp.value()));
    //cout << "next move for label prop: " << best_move_vertex << " " << best_move_set_from << " " << best_move_set_to << "\n";
    return optional<int>(get<2>(temp.value()));
}

optional<int> label_propagation_accept_weighted_random(Graph &g, SolutionRepresentation &sol) {
    optional<tri> temp;
    //map<int, tri> best;
    if (sol.num_sets() < 2) {
        return optional<int>();
    }
    else if (sol.book.b_lp.label_prop_counter == 0 || sol.book.b_lp.best_label_prop.empty()) {
        sol.book.b_lp.best_label_prop.clear();
        //set to move from and to, cost
        
        for (int v = 0; v < g.n; v++) {
            temp = find_best_move(g, sol, v);
            if (!temp.has_value()) {
                continue;
            }
            //cout << "get 2 of temp: :" << get<2>(temp.value()) << ":\n";
            sol.book.b_lp.best_label_prop.emplace_back(pair<int, int>(get<2>(temp.value()), v));
            //best[v] = temp.value();
        }
        if (sol.book.b_lp.best_label_prop.empty()) {
            return optional<int>();
        }
        sort(sol.book.b_lp.best_label_prop.begin(), sol.book.b_lp.best_label_prop.end(), cmp_greater_pair());

        //sol.book.b_lp.best_label_prop.pop_back();
        sol.book.b_lp.label_prop_counter = g.n / 2;
    } else {
        sol.book.b_lp.label_prop_counter--;
    }

    pair<int, int> op = sol.book.b_lp.best_label_prop.back();
    sol.book.b_lp.best_label_prop.pop_back();

    vector<tri> best_moves = find_best_moves(g, sol, op.second);
    //for (auto t : best_moves) {
    //    cout << get<0>(t) << " ";
    //}
    //cout << "\n";
    if (best_moves.empty()) return optional<int>();
    int r = sol.ra.weighted_random_index(5, best_moves.size(), 2);
    temp = best_moves[r];
    if (!temp.has_value()) return {};
    sol.book.b_lp.next_move = tri(op.second, get<1>(temp.value()), get<2>(temp.value()));
    //cout << "next move for label prop: " << best_move_vertex << " " << best_move_set_from << " " << best_move_set_to << "\n";
    return optional<int>(get<0>(temp.value()));
}

optional<int> label_propagation_accept_unchanged(Graph &g, SolutionRepresentation &sol, int i) {
    if (sol.num_sets() < 2) {
        return optional<int>();
    }
    //i - 1 since we want one vertex to be moved several times in escape sequence
    set<int> recently_moved = sol.book.modified_vertices.query(max(0, i - 5*g.n), max(0, i - 1));
    //for (int p : recently_moved) cout << p << " ";
    //cout << "\n";
    vector<int> choices;
    for (int j = 0; j < g.n; j++) {
        if (!(recently_moved.find(j) != recently_moved.end())) choices.push_back(j);
    }
    if (choices.empty()) return {};
    int v = choices[sol.ra.get_random_int() % choices.size()];
    optional<tri> temp = find_best_move(g, sol, v);
    if (!temp.has_value()) return {};
    sol.book.b_lp.next_move = tri(v, get<0>(temp.value()), get<1>(temp.value()));
    //cout << "next move for label prop: " << best_move_vertex << " " << best_move_set_from << " " << best_move_set_to << "\n";
    return optional<int>(get<2>(temp.value()));
}
