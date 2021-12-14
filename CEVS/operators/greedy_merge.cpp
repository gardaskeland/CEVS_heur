#include "greedy_merge.h"


int cost_add_edges(set<int> &s, Graph &g) {
    int cost = 0;
    vector<int> vec_1;
    for (int i : s) {
        vec_1.push_back(i);
    }
    for (int i = 0; i < vec_1.size() - 1; i++) {
        for (int j = i + 1; j < vec_1.size(); j++) {
            //cout << vec_1[i] << " " << vec_1[j] << "\n";
            if (!g.has_edge(vec_1[i], vec_1[j]))
                cost += 1;
        }
    }
    return cost;
}

int cost_add_edges_two_sets(set<int> &s1, set<int> &s2, Graph &g) {
    set<pair<int, int>> edges;
    int cost = 0;
    vector<int> vec_1;
    for (int i : s1) {
        vec_1.push_back(i);
    }
    for (int i = 0; i < vec_1.size() - 1; i++) {
        for (int j = i + 1; j < vec_1.size(); j++) {
            if (!g.has_edge(vec_1[i], vec_1[j]))
                cost += 1;
                edges.insert(pair<int, int>(vec_1[i], vec_1[j]));
                edges.insert(pair<int, int>(vec_1[j], vec_1[i]));
        }
    }

    vector<int> vec_2;
    for (int i : s2)
        vec_2.push_back(i);
    
    for (int i = 0; i < vec_2.size() - 1; i++) {
        for (int j = i + 1; j < vec_2.size(); j++) {
            if (!(edges.find(pair<int, int>(vec_2[i], vec_2[j])) != edges.end()) &&
                !g.has_edge(vec_2[i], vec_2[j])) {
                cost += 1;
            }
        }
    }
    return cost;
}

/**
 * Finds the number of neighbours not in s of node v.
 */
int find_neighbours_not_in_s(int v, set<int> &s, Graph &g) {
    //cout << "for vertex " << v << ":\n";
    int cost = 0;
    for (int j : g.adj[v]) {
        if (!(s.find(j) != s.end()) && g.has_edge(v, j)) {
            //cout << "count edge " << v << "," << j << "\n";
            cost += 1;
        }   
    }
    return cost;
} 

int cost_neigbour_clusters(set<int> &s, SolutionRepresentation &sol, Graph &g) {
    int cost = 0;
    //Checks the number of clusters node i is in.
    for (int i : s) {
        //cout << i << "\n";
        int num_clusters = sol.get_node_to_clusters(i).size();
        if (num_clusters > 1) {
            cost += num_clusters - 1;
        }
        else {
            //cout << "Using find_neighbours_not_in_s: " << find_neighbours_not_in_s(i, s, g) << "\n";
            cost += find_neighbours_not_in_s(i, s, g);
        }
    }
    return cost;
}

//TODO: Think about this more closely in order to make sure that it doesn't count vertices and
//edges twice etc.
//TODO: test.
//TODO: Think about if not counting edges when a vertex is in several clusters creates bias
//when using the operator.
int merge_cost(SolutionRepresentation &sol, Graph &g, int si, int sj) {
    int cost_1 = 0;
    int cost_2 = 0;
    set<int> set_1 = sol.get_set(si);
    set<int> set_2 = sol.get_set(sj);

    //cost_1 += cost_neigbour_clusters(set_1, sol, g);
    //cost_2 += cost_neigbour_clusters(set_2, sol, g);

    //Subtract common vertices between s1 and s2 as they are counted twice in
    //cost_neighbour_clusters
    int common_vertices = 0;
    for (int i : set_1) {
        if (set_2.find(i) != set_2.end()) {
            common_vertices += 1;
        }
    }

    //cout << "cost_neighbour_clusters 1: " << cost_neigbour_clusters(set_1, sol, g) << "\n";
    //cout << "cost_neighbour_clusters 2: " << cost_neigbour_clusters(set_2, sol, g) << "\n";
    //cout << "common vertices: " << common_vertices << "\n";
    //cout << "cost_add_edges_two_sets: " << cost_add_edges_two_sets(set_1, set_2, g) << "\n";
    int cost_before_merge = cost_neigbour_clusters(set_1, sol, g) 
        + cost_neigbour_clusters(set_2, sol, g) - common_vertices
        + cost_add_edges_two_sets(set_1, set_2, g);

   //cout << "Cost before merge: " << cost_before_merge << "\n";

    //calculate cost after merge
    int cost_after_merge = 0;

    set<int> merged;
    for (int i : set_1)
        merged.insert(i);
    for (int i : set_2)
        merged.insert(i);

    cost_after_merge += cost_add_edges(merged, g) + cost_neigbour_clusters(merged, sol, g);
    //cout << "cost_add_edges: " << cost_add_edges(merged, g) << "\n";
    //cout << "cost_neighbourhood_clusters: " << cost_neigbour_clusters(merged, sol, g) << "\n";
    //cout << "intermediate cost_after_merge: " << cost_after_merge << "\n";
    //Subtracting common vertices twice as these now have been counted twice in
    //cost_neighbour_clusters without being in different clusters.
    cost_after_merge -= common_vertices;


    //Counting edges that must be removed for any vertices vi that are in only one cluster after the merge
    for (int vi : merged) {
        set<int> clusters_with_vi = sol.get_node_to_clusters(vi);
        if (clusters_with_vi.size() == 2 && clusters_with_vi.find(si) != clusters_with_vi.end()
            && clusters_with_vi.find(sj) != clusters_with_vi.end()) {
            for (int w : g.adj[vi]) {
                if (!(merged.find(w) != merged.end())) {
                    cost_after_merge += 1;
                }
            }
        }
    }

    //cout << "Cost after merge: " << cost_after_merge << "\n";

    return cost_after_merge - cost_before_merge;


}

void merge(SolutionRepresentation &sol, int si, int sj) {
    sol.merge(si, sj);
}

struct revert_greedy_merge {
    int si;
    int sj;
    set<int> si_nodes;
    set<int> sj_nodes;
};

set<int> copy_set(set<int> &s){
    set<int> copy;
    for (int i : s) {
        copy.insert(i);
    } return copy;
}

void do_revert_merge(SolutionRepresentation &sol, Bookkeep &book) {
    sol.add_set_ind(book.revert_merge_ind[0], book.revert_merge_sets[0]);
    sol.add_set_ind(book.revert_merge_ind[1], book.revert_merge_sets[1]);
}

map<int, pair<int, int>> find_cost_of_merges(Graph &g, SolutionRepresentation &sol) {
    map<int, pair<int, int>> cost_of_merges;
    vector<int> indices = sol.get_set_indices();
    for (int i = 0; i < indices.size() - 1; i++) {
        for (int j = i + 1; j < indices.size(); j++) {
            cost_of_merges[merge_cost(sol, g, indices[i], indices[j])] = pair<int, int>(indices[i], indices[j]);
        }
    }
    return cost_of_merges;
}

/**
 * Return the cost difference in the total cost function of the graph after executing the merge of
 * sets with indices si and sj.
 * 
 * Deletions: When merging two sets, some edges may no longer need to be deleted: Those that
 * have one endpoint in si and one in sj.
 * 
 * Additions: When merging two sets, some edges must be added, which are for pairs of nodes
 * that are in the same set after merging and do not have an edge.
 * 
 * Splitting: When merging sets, nodes that are common to both sets must be split one time less.
 * 
 * TODO: Change so that it uses the weights of edges and vertices.
 */
int cost_diff_after_merge(Graph &g, SolutionRepresentation &sol, int si, int sj) {
    set<int> nodes_si = sol.get_set(si);
    for (int i : nodes_si) {
        //cout << "node " << i << "\n";
    }
    set<int> nodes_sj = sol.get_set(sj);
    int cost = 0;
    //vector<int> v(nodes_si.size() + nodes_sj.size());
    set<int> v_clusters;
    set<int> w_clusters;
    bool count = true;


    //cout << "ok0";
    for (int v : nodes_si) {
        //deletion
        //This makes it so that the cost of a merge can be affected even though
        //neither of the two clusters are changed. So to be completely correct,
        //we would need to calculate for all clusters. Same goes for addition.
        for (int w : g.adj[v]) {
            //if w is not in nodes_sj
            if (!(nodes_sj.find(w) != nodes_sj.end())) {
                continue;
            }
            v_clusters = sol.get_node_to_clusters(v);
            w_clusters = sol.get_node_to_clusters(w);
            count = true;
            //Since both vertices are in the same cluster, it is not deleted.
            for (int c_v : v_clusters) {
                if (w_clusters.find(c_v) != w_clusters.end()) {
                    count = false;
                    break;
                }
            }
            if (count) {
                //Since after merge there is no need to delete the edge
                cost -= g.get_edge_cost(v, w);
            }
        }
        //addition. Not checking if two nodes are in the same cluster, so we don't need to
        //count the addition
        //cout << "ok2";
        for (int w : nodes_sj) {
            v_clusters = sol.get_node_to_clusters(v);
            w_clusters = sol.get_node_to_clusters(w);
            count = true;
            //Since both vertices are in the same cluster, it is not deleted.
            for (int c_v : v_clusters) {
                if (w_clusters.find(c_v) != w_clusters.end()) {
                    count = false;
                    break;
                }
            }
            if (count && !g.has_edge(v, w)) {
                cost += g.get_edge_cost(v, w);
            }
        }
        //cout << "ok3\n";
        if (nodes_sj.find(v) != nodes_sj.end()) {
            // vertex v must be split one time less after merge.
            cost -= g.get_node_weight(v);
        }
        //cout << "ok4\n";
    }
    //cout << "ok5\n";
    return cost;
}

/**
 * @brief Uses the difference between before and after merge as measure instead of the local cost.
 * Returns a sorted map of the cost of merging each pair mapped to the pair.
 * 
 * @param g 
 * @param sol 
 * @return map<int, pair<int, int>> 
 */
map<int, pair<int, int>> find_cost_of_merges_diff(Graph &g, SolutionRepresentation &sol) {
    map<int, pair<int, int>> cost_of_merges;
    vector<int> indices = sol.get_set_indices();
    int temp = 0;
    if (sol.book.b_merge.last_merge_operation == -1) {
        cout << "precomputing map_merge_cost\n";
        //cout << indices.size();
        for (int i = 0; i < indices.size() - 1; i++) {
            for (int j = i + 1; j < indices.size(); j++) {
                temp = cost_diff_after_merge(g, sol, indices[i], indices[j]);
                //cout << "1\n";
                sol.book.b_merge.map_merge_cost[pair<int, int>(indices[i], indices[j])] = temp; 
                //cout << "2\n";
                sol.book.b_merge.pq_merge_cost.push(pair<int, pair<int,int>>(temp, pair<int, int>(indices[i], indices[j])));
                //cout << "3\n";
            }
        }
        //cout << "ok\n";
    } else {
        //Updating the cost of merging clustered altered since last time with new clusters.
        set<int> modified_clusters = sol.book.modified_clusters.query(sol.book.b_merge.last_merge_operation, sol.book.operation_number - 1);
        set<int> indices;
        for (int i : sol.get_set_indices()) {
            indices.insert(i);
        }
        //cout << "in modified clusters: ";
        for (int c : modified_clusters) {
            //cout << c << ", ";
            if (!(indices.find(c) != indices.end())) continue;
            
            for (int i : indices) {
                //sol.print_solution();
                //cout << 15 << "\n";
                if (c == i) {
                    continue;
                }
                //cout << c << " and " << i << "\n";
                temp = cost_diff_after_merge(g, sol, c, i);
                sol.book.b_merge.map_merge_cost[minmax(c, i)] = temp;
                sol.book.b_merge.pq_merge_cost.push(pair<int, pair<int, int>>(temp, pair<int, int>(minmax(c, i))));
                //sol.print_solution();
            }
        }
        //cout << "\n";
    }
    /*
    sol.book.b_merge.empty_pq();
    for (int i = 0; i < indices.size() - 1; i++) {
        for (int j = i + 1; j < indices.size(); j++) {
            //Picks only one of the least valued. Doesn't store several. Should use pq?
            sol.book.b_merge.pq_merge_cost.push(pair<int, pair<int,int>>(sol.book.b_merge.map_merge_cost[pair<int, int>(indices[i], indices[j])],
            pair<int, int> (indices[i], indices[j])));
            cost_of_merges[sol.book.b_merge.map_merge_cost[pair<int,int>(indices[i],indices[j])]] = pair<int, int>(indices[i], indices[j]);
        }
    }
    */
    return cost_of_merges;
}


void greedy_merge(Graph &g, SolutionRepresentation &sol, Bookkeep &book) {
    //Stores the cost of each merge. Negative means total cost improves.
    if (sol.num_sets() <= 1) {
        return;
    }
    map<int, pair<int, int>> cost_of_merges = find_cost_of_merges(g, sol);

    map<int, pair<int, int>>::iterator it = cost_of_merges.begin();
    pair<int, int> to_merge = it->second;
    cout << "Merging sets " << to_merge.first << " and " << to_merge.second << "\n";

    book.revert_merge_ind[0] = to_merge.first;
    book.revert_merge_ind[1] = to_merge.second;
    book.revert_merge_sets[0] = sol.get_set(to_merge.first);
    book.revert_merge_sets[1] = sol.get_set(to_merge.second);


    /**
     * revert_greedy_merge rgm;
    rgm.si = to_merge.first;
    rgm.sj = to_merge.second;
    rgm.si_nodes = copy_set(sol.get_set(rgm.si));
    rgm.sj_nodes = copy_set(sol.get_set(rgm.sj));
    */

    sol.merge(to_merge.first, to_merge.second);
}



//TODO: 
// Implement pq update and selection.
//May want to have on operator that uses local cost (best results for case 001) and one
//operator that uses cost_diff_after_merge.
int weighted_random_merge(Graph &g, SolutionRepresentation &sol) {
    if (sol.num_sets() <= 1) {
        return 0;
    }

    //currently the side effects updating sol.book.b_merge are useful here.
    map<int, pair<int, int>> cost_of_merges = find_cost_of_merges_diff(g, sol);
    //cout << "finished find_cost_of_merges_diff\n";

    int ind = weighted_random_index(10, sol.book.b_merge.pq_merge_cost.size(), 1.7);

    //cout << "ind = " << ind << "\n";
    //cout << "ind: " << ind << "\n";
    //cout << "size of cost_of_merges: " << cost_of_merges.size() << "\n";

    int counter = 0;
    pair<int, pair<int, int>> next;
    vector<pair<int, pair<int, int>>> to_reinsert;
    while (counter < min(ind + 1, sol.num_sets()*(sol.num_sets() - 1)/2)) {
        while (true) {
            //cout << "ok" << counter << "\n";
            next = sol.book.b_merge.pq_merge_cost.top();
            sol.book.b_merge.pq_merge_cost.pop();
            if (sol.book.b_merge.map_merge_cost[next.second] != next.first) {
                 continue;
            }
            break;
        }
        to_reinsert.push_back(next);
        counter += 1;
    }

    for (pair<int, pair<int, int>> p : to_reinsert) {
        sol.book.b_merge.pq_merge_cost.push(p);
    }

    pair<int, int> to_merge = next.second;
    int cost = next.first;
    sol.book.b_merge.si = to_merge.first;
    sol.book.b_merge.sj = to_merge.second;
    //sol.book.b_merge.map_merge_cost[to_merge] = cost;
    //pair<int, pair<int, int>> to_pq(cost, to_merge);
    //sol.book.b_merge.pq_merge_cost.push(to_pq);
    return cost;

    /**
    book.revert_merge_ind[0] = to_merge.first;
    book.revert_merge_ind[1] = to_merge.second;
    book.revert_merge_sets[0] = sol.get_set(to_merge.first);
    book.revert_merge_sets[1] = sol.get_set(to_merge.second);
    */

    //sol.merge(to_merge.first, to_merge.second);

}

