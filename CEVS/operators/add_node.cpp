#include "add_node.h"

double relative_out_degree(Graph g, SolutionRepresentation sol, int si) {
    int count_out_edges = 0;
    set<int> nodes_in_si = sol.get_set(si);
    //cout << "Start new test case\n";
    for (int node : nodes_in_si) {
        vector<int> neighbours = g.adj[node];
        for (int neighbour : neighbours) {
            if (nodes_in_si.find(neighbour) != nodes_in_si.end()) {
                continue;
            }
            else {
                //cout << "count node " << neighbour << "\n";
                count_out_edges += 1;
            }
        }
    }
    return count_out_edges / nodes_in_si.size();

}

int highest_relative_out_degree(Graph g, SolutionRepresentation sol) {
    map<int, int> relative_out_degrees;
    for (int i : sol.get_set_indices()) {
        relative_out_degrees[relative_out_degree(g, sol, i)] = i;
    }
    map<int, int>::reverse_iterator it = relative_out_degrees.rbegin();
    return it->second;
}

int add_node_to_set_cost(Graph g, SolutionRepresentation sol, set<int> set_nodes, int v) {
    int edges_between = 0;
    for (int i : set_nodes) {
        if (g.has_edge(i, v)) {
            edges_between += 1;
        }
    // (n - edges_between) - edges_between, operations needed after adding minus operations needed before
    } return set_nodes.size() - 2*edges_between;
}

map<int, int> best_nodes_to_add(Graph g, SolutionRepresentation sol, int si) {
    map<int, int> results;
    set<int> marked_neighbours;
    set<int> set_nodes = sol.get_set(si);
    for (int v : set_nodes) {
        for (int w : g.adj[v]) {
            if (set_nodes.find(w) != set_nodes.end() || marked_neighbours.find(w) != marked_neighbours.end()) {
                continue;
            } else {
                results[add_node_to_set_cost(g, sol, set_nodes, w)] = w;
                marked_neighbours.insert(w);
            }
        }
    }
    return results;
}

struct revert_add_node {
    int v;
    int si;
};

void do_revert_add_node(SolutionRepresentation sol, revert_add_node ran) {
    sol.remove(ran.v, ran.si);
}

void add_node(Graph g, SolutionRepresentation &sol) {
    int si = highest_relative_out_degree(g, sol);
    map<int, int> best_nodes = best_nodes_to_add(g, sol, si);
    revert_add_node rad;
    rad.v = best_nodes.begin()->second;
    rad.si = si;
    sol.add(best_nodes.begin()->second, si);
    sol.print_solution();
}