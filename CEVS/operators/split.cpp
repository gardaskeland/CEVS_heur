#include "split.h"

//Returns true if the edge i j is covered by another cluster than si, that is i and j is 
//both in the other cluster sj.
bool edge_covered_by_cluster(int i, int j, Graph &g, SolutionRepresentation &sol, int si) {
    set<int> clusters_with_i = sol.get_node_to_clusters(i);
    set<int> clusters_with_j = sol.get_node_to_clusters(j);
    for (int sj : clusters_with_j) {
        if (si == sj) {
            continue;
        }
        if (clusters_with_i.find(sj) != clusters_with_i.end()) {
            return true;
        }
    }
    return false;
}

//rewrite, is slow
int compute_inner_cost (Graph &g, SolutionRepresentation &sol, int si) {
    vector<int> nodes;
    for (int u : sol.get_set(si)) {
        nodes.push_back(u);
    }
    //chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    int inner_cost = 0;
    for (int i = 0; i < nodes.size() - 1; i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            if (!g.has_edge(nodes[i], nodes[j])) {
                if (!edge_covered_by_cluster(nodes[i], nodes[j], g, sol, si)) inner_cost += g.get_edge_cost(nodes[i], nodes[j]);
                //inner_cost += g.get_edge_cost(nodes[i], nodes[j]);
            }
        }
    }
    //chrono::steady_clock::time_point end  = chrono::steady_clock::now();
    //double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    //cout << "time used in seconds on computation of inner cost: " << time_elapsed / 1000000 << "\n";
    return inner_cost / nodes.size();
}

//This does not account for edges being covered by other sets than si.
int compute_inner_cost_alt(Graph &g, SolutionRepresentation &sol, int si) {
    int num_edges = 0;
    set<pair<int, int>> edges;
    for (int u : sol.get_set(si)) {
        for (int v : g.adj[u]) {
            if (edges.find(pair<int, int>(u, v)) != edges.end()) continue;
            num_edges += 1;
            edges.insert(pair<int, int>(u, v));
            edges.insert(pair<int, int>(v, u));
        }
    }
    int n = sol.get_set(si).size();
    return ((n * (n-1)) / 2 - num_edges) / n;
}

//Noticably, this is really tidy compared to the corresponding function in greedy_merge.cpp.
//Code quality can be improved quite a bit in that file and others, seemingly.
void find_inner_costs(Graph &g, SolutionRepresentation &sol) {
    int temp = 0;
    set<int> indices = sol.get_set_indices_as_set();
    if (sol.book.b_split.last_split_operation == -1) {
        cout << "precompute inner costs\n";
        for (int si : sol.get_set_indices()) {
            temp = compute_inner_cost(g, sol, si);
            sol.book.b_split.inner_cost[si] = temp;
            sol.book.b_split.pq_inner_cost.push(pair<int, int>(temp, si));
        }
    } else {
    
        set<int> modified_clusters = sol.book.modified_clusters.query(sol.book.b_split.last_split_operation, sol.book.operation_number - 1);
        for (int si : modified_clusters) {
            if (indices.find(si) != indices.end()) {
                temp = compute_inner_cost(g, sol, si);
                sol.book.b_split.inner_cost[si] = temp;
                sol.book.b_split.pq_inner_cost.push(pair<int, int>(temp, si));
            }
        }
    }
}

int cost_of_split(Graph &g, SolutionRepresentation &sol, set<int> set_1, set<int> set_2, int si) {
    int cost_of_cut = 0;
    for (int i : set_1) {
        for (int j : set_2) {
            //Should check whether the edge is in the graph before counting deletion.
            if (g.has_edge(i, j) && sol.get_co_occurence(i, j) < 2) {
                cost_of_cut += g.get_edge_cost(i, j);
            }
            //g does not have the edge, so it have been added and can now be deleted.
            else if (sol.get_co_occurence(i, j) < 2) {
                cost_of_cut -= g.get_edge_cost(i, j);
            }
        }
    }
    return cost_of_cut;
}

//This hardly works at all as we get only one node in clique_2 most times.
pair<int, pair<set<int>, set<int>>> clique_split(Graph &g, SolutionRepresentation &sol, int si) {
    set<int> si_nodes = sol.get_set(si);
    set<int> clique_1;
    set<int> clique_2;
    set<int> marked;
    set<int> in_queue;
    queue<int> q;
    bool done = false;
    for (int u : si_nodes) {
        for (int v : si_nodes) {
            if (u == v || g.has_edge(u, v)) {
                continue;
            }
            in_queue.insert(u); in_queue.insert(v);
            q.push(u); q.push(v);
            done = true;
            break;
        }
        if (done) break;
    }
    //The case where si is a clique.
    if (q.empty()) return pair<int, pair<set<int>, set<int>>>(-1, pair<set<int>, set<int>>(set<int>(), set<int>()));
    while(!q.empty()) {
        int u = q.front(); q.pop();
        if (marked.find(u) != marked.end()) continue;
        marked.insert(u);
        int score_1 = 0;
        int score_2 = 0;
        for (int v : g.adj[u]) {
            if (!(si_nodes.find(v) != si_nodes.end())) continue;
            //edge additions
            if (clique_1.find(v) != clique_1.end() && !g.has_edge(u, v)) score_1 += 1;
            if (clique_2.find(v) != clique_2.end() && !g.has_edge(u, v)) score_2 += 1;
            //edge deletions
            if (in_queue.find(v) != in_queue.end()) continue;
            q.push(v);
            in_queue.insert(v);
        }
        //edge
        for (int v : clique_1) {
            if (g.has_edge(u, v)) score_2 += 1;
        }
        for (int v : clique_2) {
            if (g.has_edge(u, v)) score_1 += 1;
        }
        if (score_2 / (double)clique_2.size() > score_1 / (double)clique_1.size()) {
            clique_1.insert(u);
        } else if (score_2 / (double)clique_2.size() < score_1 / (double)clique_1.size()) {
            clique_2.insert(u);
        }
        else {
            if (clique_1.size() > clique_2.size()) {
                clique_2.insert(u);
            } else {
                clique_1.insert(u);
            }
        }
    }
    return pair<int, pair<set<int>, set<int>>> (0, pair<set<int>, set<int>>(clique_1, clique_2));
}

bool is_connected_component(Graph &g, SolutionRepresentation &sol, int si) {
    set<int> seen;
    queue<int> q;
    set<int> si_nodes = sol.get_set(si);
    q.push(*si_nodes.begin());

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        seen.insert(u);
        for (int v : g.adj[u]) {
            if (seen.find(v) != seen.end()) continue;
            if (si_nodes.find(v) != si_nodes.end()) {
                q.push(v);
            }
        }
    }
    if (seen.size() < si_nodes.size()) return false;
    else return true;
    
}

optional<int> random_choice_split(Graph &g, SolutionRepresentation &sol) {
    vector<int> set_indices = sol.get_set_indices();
    int r = rand() % set_indices.size();
    int si = set_indices[r];
    //cout << "r: " << r << "\n";
    if (sol.get_set(si).size() <= 1 || !is_connected_component(g, sol, si)) {
        sol.book.b_split.si = -1;
        return {};
    }
    sol.book.b_split.si = si;
    pair<int, pair<set<int>, set<int>>> min_cut = find_min_cut(g, sol, si);
    int cost_of_cut = cost_of_split(g, sol, min_cut.second.first, min_cut.second.second, si);
    sol.book.b_split.cut = min_cut.second;
    return optional(cost_of_cut);
}

int greedy_split(Graph &g, SolutionRepresentation &sol, string f) {
    //chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    //cout <<" 1\n";
    //sol.print_solution();
    find_inner_costs(g, sol);

    //chrono::steady_clock::time_point end = chrono::steady_clock::now();
    //double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    //cout << "time used in seconds on inner cost: " << time_elapsed / 1000000 << "\n";
    //cout << "2 \n";
    //sol.print_solution();

    int ind = weighted_random_index(10, sol.book.b_split.pq_inner_cost.size(), 1.7);
    int counter = 0;
    pair<int, int> next;
    vector<pair<int, int>> to_reinsert;
    //cout << "new\n";
    while (counter < min(ind + 1, sol.num_sets())) {
        while(true) {
            next = sol.book.b_split.pq_inner_cost.top();
            //cout << "next.first: " << next.first << ", ";
            //cout << "next.second: " << next.second << "\n";
            sol.book.b_split.pq_inner_cost.pop();
            //removing pair with wrong inner cost for set of sol.
            if (sol.book.b_split.inner_cost[next.second] != next.first) {
                continue;
            }
            break;
        }
        to_reinsert.push_back(next);
        counter += 1;
    }
    for (pair<int, int> p : to_reinsert) {
        sol.book.b_split.pq_inner_cost.push(p);
    }

    //cout << "3\n";
    //sol.print_solution();
    set<int> indices = sol.get_set_indices_as_set();
    if (!(indices.find(next.second) != indices.end())) {  
        sol.book.b_split.si = -1;
        return 1000000;
    }
    if (sol.get_set(next.second).size() <= 1) {
        sol.book.b_split.si = -1;
        return 1000000;
    }
    sol.book.b_split.si = next.second;

    //cout << "4\n";
    //sol.print_solution();
    pair<int, pair<set<int>, set<int>>> min_cut;

    //set<int> modified_clusters = sol.book.modified_clusters.query(sol.book.b_split.last_split_operation, sol.book.operation_number - 1);

    //Problem with his: May not find the min-cut since karger runs only n times. So instead we find a min-cut with 1/e prob. every time the operation is called,
    //which after some operations is likely to result in the min-cut.
    /**
    if (!(sol.book.b_split.recent_cuts.find(next.second) != sol.book.b_split.recent_cuts.end())) {
        min_cut = find_min_cut(g, sol, next.second);
        sol.book.b_split.recent_cuts[next.second] = min_cut.second;
    }
    //if cluster has changed since last cut was found
    else if (modified_clusters.find(next.second) != modified_clusters.end()) {
        min_cut = find_min_cut(g, sol, next.second);
        sol.book.b_split.recent_cuts[next.second] = min_cut.second;
    }
    else {
        //cout << "operation number: " << sol.book.operation_number - 1 << "\n";
        min_cut = pair<int, pair<set<int>, set<int>>>(0, sol.book.b_split.recent_cuts[next.second]);
    }
    */

    if (f.compare("min_cut") == 0) {
        min_cut = find_min_cut(g, sol, next.second);
    } else if (f.compare("clique_split") == 0) {
        min_cut = clique_split(g, sol, next.second);
        if (min_cut.first == -1) return 10000000;
    } else {
        cout << "ERROR: Invalid input string f to greedy split: " << f << "\n";
    }

    //Cost of removing edges - cost of adding. Does not yet consider that edges may be covered
    //by other sets.
    int cost_of_cut = cost_of_split(g, sol, min_cut.second.first, min_cut.second.second, next.second);
    /*
    for (int i : min_cut.second.first) {
        for (int j : min_cut.second.second) {
            //Should check whether the edge is in the graph before counting deletion.
            if (g.has_edge(i, j) && !edge_covered_by_cluster(i, j, g, sol, next.second)) {
                cost_of_cut += g.get_edge_cost(i, j);
            }
            //g does not have the edge, so it have been added and can now be deleted.
            else if (!edge_covered_by_cluster(i, j, g, sol, next.second)) {
                cost_of_cut -= g.get_edge_cost(i, j);
            }
        }
    }
    */
    //Second part reconstructed from inner_cost.
    //int cost = cost_of_cut - next.first * sol.get_set(next.second).size();
    sol.book.b_split.cut = min_cut.second;
    return cost_of_cut;
    
}

void do_split(SolutionRepresentation &sol) {
    pair<set<int>, set<int>> p = sol.book.b_split.cut;
    sol.disjoint_split(sol.book.b_split.si, p.first, p.second);
}