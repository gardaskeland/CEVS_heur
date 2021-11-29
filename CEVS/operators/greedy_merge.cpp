#include "greedy_merge.h"


int cost_add_edges(set<int> s, Graph g) {
    int cost = 0;
    vector<int> vec_1;
    for (int i : s) {
        vec_1.push_back(i);
    }
    for (int i = 0; i < vec_1.size() - 1; i++) {
        for (int j = i + 1; j < vec_1.size(); j++) {
            if (!g.has_edge(vec_1[i], vec_1[j]))
                cost += 1;
        }
    }
    return cost;
}

int cost_add_edges_two_sets(set<int> s1, set<int> s2, Graph g) {
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
int find_neighbours_not_in_s(int v, set<int> s, Graph g) {
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

int cost_neigbour_clusters(set<int> s, SolutionRepresentation sol, Graph g) {
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
int merge_cost(SolutionRepresentation sol, Graph g, int si, int sj) {
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
    //cout << "cost_neighbourhood_clusters" << cost_neigbour_clusters(merged, sol, g) << "\n";
    //cout << "intermediate cost_after_merge: " << cost_after_merge << "\n";
    //Subtracting common vertices twice as these now have been counted twice in
    //cost_neighbour_clusters without being in different clusters.
    //cost_after_merge -= 2 * common_vertices;


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