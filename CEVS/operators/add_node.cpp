#include "add_node.h"

double relative_out_degree(Graph &g, SolutionRepresentation &sol, int si) {
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

map<double, int> get_relative_out_degrees(Graph &g, SolutionRepresentation &sol) {
    map<double, int> relative_out_degrees;
    for (int i : sol.get_set_indices()) {
        relative_out_degrees[relative_out_degree(g, sol, i)] = i;
    }
    return relative_out_degrees;
}

int highest_relative_out_degree(Graph &g, SolutionRepresentation &sol) {
    map<double, int> relative_out_degrees = get_relative_out_degrees(g, sol);
    map<double, int>::reverse_iterator it = relative_out_degrees.rbegin();
    return it->second;
}

int add_node_to_set_cost(Graph &g, SolutionRepresentation &sol, set<int> &set_nodes, int v) {
    int edges_between = 0;
    for (int i : set_nodes) {
        if (g.has_edge(i, v)) {
            edges_between += 1;
        }
    // (n - edges_between) - edges_between, operations needed after adding minus operations needed before
    } return set_nodes.size() - 2*edges_between;
}

map<int, int> best_nodes_to_add(Graph &g, SolutionRepresentation &sol, int si) {
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

void do_revert_add_node(SolutionRepresentation &sol, Bookkeep &book) {
    sol.remove(book.revert_add_node[0], book.revert_add_node[1]);
}

void add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book) {
    int si = highest_relative_out_degree(g, sol);
    map<int, int> best_nodes = best_nodes_to_add(g, sol, si);
    sol.add(best_nodes.begin()->second, si);
    book.revert_add_node[0] = best_nodes.begin()->second;
    book.revert_add_node[1] = si;
    //sol.print_solution();
}

//Possibly not working yet.
void weighted_random_add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book) {
    map<double, int> out_deg = get_relative_out_degrees(g, sol);
    int ind = weighted_random_index(10, out_deg.size(), 2);
    
    int counter = 0;
    map<double, int>::iterator it = out_deg.begin();
    while (counter != ind) {
        it++;
        counter += 1;
    }
    int si = it->second;

    map<int, int> best_nodes = best_nodes_to_add(g, sol, si);
    ind = weighted_random_index(10, out_deg.size(), 2);

    counter = 0;
    map<int, int>::iterator it2 = best_nodes.begin();
    while (counter != ind) {
        it2++;
        counter += 1;
    }
    sol.add(it2->second, si);
    book.revert_add_node[0] = it2->second;
    book.revert_add_node[1] = si;




}