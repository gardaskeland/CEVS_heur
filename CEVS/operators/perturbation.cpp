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
    set<int> sets = sol.get_node_to_clusters(v);
    int si = choose_random_set_entry(sets);
    int cost = removal_cost(g, sol, si, v);
    sol.remove(v, si);
    set<int> to_add;
    to_add.insert(v);
    sol.add_set(to_add);
    //+1 since added node
    return cost + 1;
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
        //since we add two nodes.
        return cost + 2;
    } else {
        //since we add two nodes and no longer have to delete an edge: 2 - 1.
        return cost + 1;
    }
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