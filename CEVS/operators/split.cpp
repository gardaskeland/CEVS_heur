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

int compute_inner_cost (Graph &g, SolutionRepresentation &sol, int si) {
    vector<int> nodes;
    for (int u : sol.get_set(si)) {
        nodes.push_back(u);
    }
    int inner_cost = 0;
    for (int i = 0; i < nodes.size() - 1; i++) {
        for (int j = i + 1; j < nodes.size(); j++) {
            if (!g.has_edge(i, j)) {
                if (!edge_covered_by_cluster(i, j, g, sol, si)) inner_cost += g.get_edge_cost(i, j);
            }
        }
    }
    return inner_cost / nodes.size();
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

int greedy_split(Graph &g, SolutionRepresentation &sol) {
    //cout <<" 1\n";
    //sol.print_solution();
    find_inner_costs(g, sol);
    //cout << "2 \n";
    //sol.print_solution();
    int ind = weighted_random_index(10, sol.book.b_split.pq_inner_cost.size(), 2);
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
    /**
    set<int> modified_clusters = sol.book.modified_clusters.query(sol.book.b_split.last_split_operation, sol.book.operation_number - 1);
    if (modified_clusters.find(next.second) != modified_clusters.end() || 
        !(sol.book.b_split.recent_cuts.find(next.second) != sol.book.b_split.recent_cuts.end())) {
        min_cut = find_min_cut(g, sol, next.second);
        sol.book.b_split.recent_cuts[next.second] = min_cut.second;
    }
    else {
        cout << "operation number: " << sol.book.operation_number - 1 << "\n";
        min_cut = pair<int, pair<set<int>, set<int>>>(0, sol.book.b_split.recent_cuts[next.second]);
    }
    */
    min_cut = find_min_cut(g, sol, next.second);

    //Cost of removing edges - cost of adding. Does not yet consider that edges may be covered
    //by other sets.
    int cost_of_cut = 0;
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
    //Second part reconstructed from inner_cost.
    //int cost = cost_of_cut - next.first * sol.get_set(next.second).size();
    sol.book.b_split.cut = min_cut.second;
    return cost_of_cut;
    
}