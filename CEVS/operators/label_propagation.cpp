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
        if ((best_set_cost > 0 && vertex_cost + g.get_node_weight(u) <= 0) || best_set_add == -1) {
            set<int> to_add;
            to_add.insert(u);
            sol.add_set(to_add);
            //0 for adding, all cost measured in removal.
            //+1 since we add back a node.
            cost += vertex_cost + g.get_node_weight(u);
            //if (original_cost + cost - sol.cost_solution(g) != sol_diff) {
            //    sol_diff = original_cost + cost - sol.cost_solution(g);
            //    cout << "change in sol diff when removing " << u << " from " << best_set_remove << " and isolating it\n";
            //}
            continue;
        }
        
        //cout << "ok\n";
        vertex_cost += best_set_cost;
        //If it's cheapest to leave the vertex as is.
        if (vertex_cost > 0) {
            //add the node back to original set
            sol.add(u, best_set_remove);
            //if (original_cost + cost - sol.cost_solution(g) != sol_diff) {
            //    sol_diff = original_cost + cost - sol.cost_solution(g);
            //    cout << "change in sol diff when removing " << u << " from " << best_set_remove << " and adding it back \n";
            //}
            continue;
        };
        sol.add(u, best_set_add);
        cost += vertex_cost;
        //if (original_cost + cost - sol.cost_solution(g) != sol_diff) {
        //    sol_diff = original_cost + cost - sol.cost_solution(g);
        //    cout << "change in sol diff when removing " << u << " from " << best_set_remove << " and adding it to " << best_set_add << "\n";
        //}
        //cout << "for node " << u << ":\n";
        //sol.print_solution();
        //cout << "current cost: " << sol.cost_solution(g) << "\n";
        //cout << "estimated cost: " << original_cost + cost << "\n\n";
    }
    return cost;
}

/**
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
        set<int> set_v = sol.get_node_to_clusters(v);
        set<int> neighbourhood = neighbour_clusters(g, sol, v);
        for (int s0 : set_v) {
            set<pair<int, int>> edges_unadded;
            set<pair<int, int>> edges_removed;
            set<int> s0_nodes = sol.get_set(s0);
            for (int u : s0_nodes) {
                if (u == v) continue;
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
                for (pair<int,int> e : edges_unadded) {
                    if (!(edge_unadd_ignore.find(e) != edge_unadd_ignore.end())) {
                    op_cost -= g.get_edge_cost(e.first, e.second);
                    }
                }
                for (pair<int, int> e : edges_removed) {
                    if (!(edge_remove_ignore.find(e) != edge_remove_ignore.end())) {
                        op_cost += g.get_edge_cost(e.first, e.second);
                    }
                }
                for (pair<int, int> e : edges_readded) {
                    op_cost -= g.get_edge_cost(e.first, e.second);
                }
                for (pair<int, int> e : edges_added) {
                    op_cost += g.get_edge_cost(e.first, e.second);
                }
                tuple<int, int, int> move(s0, v, si);
                sol.book.b_lp.move_cost[move] = op_cost;
                sol.book.b_lp.pq_move_cost.push(pair<int, tuple<int, int, int>>(op_cost, move));
            }
        }
    }

}


void update_cost(int s0, int v, int si, int edge_cost, SolutionRepresentation &sol) {
    tuple<int, int, int> move(s0, v, si);
    int new_cost = sol.book.b_lp.move_cost[move] + edge_cost;
    sol.book.b_lp.after_move_change.push_back(pair<tuple<int, int, int>, int>(move, new_cost));
}


bool check_node_is_neighbour_of_set(Graph &g, int v, set<int> &s) {
    for (int w : g.adj[v]) {
        if (s.find(w) != s.end()) return true;
    }
    return false;
}


optional<int> find_cost_changes(Graph &g, SolutionRepresentation &sol, int s0, int v, int si, int cost) {
    int cost_add_v_to_s0 = 0;
    set<int> si_nodes = sol.get_set(si);

    if (!check_node_is_neighbour_of_set(g, v, si_nodes)) return {};

    set<int> s0_nodes = sol.get_set(s0);
    set<int> s0_neighbours;
    set<int> v_sets = sol.get_node_to_clusters(v);

    if (si_nodes.find(v) != si_nodes.end()) return optional(cost); //No more changes needed.

    for (int u : s0_nodes) {
        for (int w : g.adj[u]) {
            if (!(s0_nodes.find(w) != s0_nodes.end())) s0_neighbours.insert(w);
        }
    }
    for (int u : s0_nodes) {
        if (u == v) continue;
        if (sol.book.b_lp.co_occurence.find(minmax(v, u)) != sol.book.b_lp.co_occurence.end() && sol.book.b_lp.co_occurence[minmax(v, u)] > 1) continue;
            int edge_cost = 0;
            //seems counterintuitive, but is correct. Is the change in cost of executing operations involving v and u.
        if (g.has_edge(v, u)) {
            edge_cost -= g.get_edge_cost(v, u);
        } else {
            edge_cost += g.get_edge_cost(v, u);
        }
        cost_add_v_to_s0 += edge_cost;
        //change the cost of adding vertices to s0.
        for (int neighbour : s0_neighbours) {
            if (sol.book.b_lp.co_occurence[minmax(neighbour, v)] > 1) continue; // || si_nodes.find(neighbour) != si_nodes.end()) continue;
            set<int> neighbour_sets = sol.get_node_to_clusters(neighbour);
            for (int sj : neighbour_sets) {
                update_cost(sj, neighbour, si, edge_cost, sol);
            }
        }

        //When moving u out of s0, no longer count the edge between u and v.
        set<int> u_neighbourhood = neighbour_clusters(g, sol, u);
        for (int neighbour : u_neighbourhood) {
            if (neighbour == si) continue;
            update_cost(s0, u, neighbour, -edge_cost, sol);
        }

        //change the cost of adding v to sets with u for vertices u in s0 unless u is in si (or cooccurencematrix > 1).
        //don't need to check if u and v is in the same set as else we would have continued on second line of for-loop
        set<int> u_sets = sol.get_node_to_clusters(u);
        for (int sj : u_sets) {
            for (int sk : v_sets) {
                if (sj == s0 || sj == si || sk == s0 || sk == si) continue;
                update_cost(sk, v, sj, edge_cost, sol);
            }
        }
    }
    for (int sj : v_sets) {
        if (sj == s0) continue;
        update_cost(sj, v, s0, cost_add_v_to_s0, sol);
    }
    //cost of adding v back
    update_cost(si, v, s0, cost_add_v_to_s0, sol);

    int cost_remove_v_from_si = 0;
    set<int> si_neighbours;
    for (int w : si_nodes) {
        for (int x : g.adj[w]) {
            if (!(si_nodes.find(x) != si_nodes.end())) si_neighbours.insert(x);
        }
    }

    //Now consider adding v to si.
    for (int w : si_nodes) {
        if (sol.book.b_lp.co_occurence.find(minmax(v, w)) != sol.book.b_lp.co_occurence.end() && sol.book.b_lp.co_occurence[minmax(v, w)] > 1 \
            || s0_nodes.find(w) != s0_nodes.end()) continue;
        int edge_cost = 0;
        if (g.has_edge(v, w)) edge_cost += g.get_edge_cost(v, w);
        else edge_cost -= g.get_edge_cost(v, w);
        cost_remove_v_from_si += edge_cost;
        
        for (int neighbour : si_neighbours) {
            if (sol.book.b_lp.co_occurence[minmax(neighbour, v)] > 1 ) continue; //|| s0_nodes.find(neighbour) != s0_nodes.end()) continue;
            set<int> neighbour_sets = sol.get_node_to_clusters(neighbour);
            for (int sj : neighbour_sets) {
                update_cost(sj, neighbour, si, edge_cost, sol);
            }
        }

        //Operators moving w out of si.
        set<int> w_neighbourhood = neighbour_clusters(g, sol, w);
        for (int neighbour : w_neighbourhood) {
            if (neighbour == s0) continue;
            update_cost(si, w, neighbour, -edge_cost, sol);
        }

        //change the cost of adding v to sets with w for vertices w in si unless u is in si (or cooccurencematrix > 1).
        //don't need to check if u and v is in the same set as else we would have continued on second line of for-loop
        set<int> w_sets = sol.get_node_to_clusters(w);
        for (int sj : w_sets) {
            for (int sk : v_sets) {
                if (sj == s0 || sj == si || sk == s0 || sk == si) continue;
                update_cost(sk, v, sj, edge_cost, sol);
            }
        }
    }
    for (int sj : v_sets) {
        if (sj == s0) continue;
        update_cost(sj, v, si, -cost_remove_v_from_si, sol);
    }

    return optional(cost);
}


optional<int> best_vertex_move(Graph &g, SolutionRepresentation &sol) {
    if (sol.book.b_lp.pq_move_cost.size() == 0) precompute_b_lp(g, sol);
    pair<int, tuple<int, int, int>> next;
    while (true) {
        if (sol.book.b_lp.pq_move_cost.size() == 0) return {};
        next = sol.book.b_lp.pq_move_cost.top();
        sol.book.b_lp.pq_move_cost.pop();
        if (sol.book.b_lp.move_cost.find(next.second) != sol.book.b_lp.move_cost.end() && next.first == sol.book.b_lp.move_cost[next.second]) break;
    }
    //next.second = operation, next.first = cost of operation
    sol.book.b_lp.s0 = get<0>(next.second);
    sol.book.b_lp.v = get<1>(next.second);
    sol.book.b_lp.s1 = get<2>(next.second);

    return find_cost_changes(g, sol, get<0>(next.second), get<1>(next.second), get<2>(next.second), next.first);
}

optional<int> do_vertex_move(SolutionRepresentation &sol) {

}




    //Know we return optional(next.first). Make changes.

    //if we remove v from s0: cooccurence -, adding v to s0 becomes an operation that we must calculate the cost of.


    //[when adding v to si, we must remove all operations adding v to si.] dubious.
    //Also change cost of operations adding v to sets with u for vertices u in si.
    //Also change cost of operations adding u to sets with v. All of this given cooccurence == 0.

    //When executing operation: update b_lp with changes to the operations.

*/