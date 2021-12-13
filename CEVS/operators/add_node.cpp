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
                count_out_edges += g.get_edge_cost(node, neighbour);
            }
        }
    }
   // cout << "nodes in si size: " << nodes_in_si.size() << "\n";
    //if (nodes_in_si.size() == 0) {return numeric_limits<int>::max();}
    return count_out_edges / nodes_in_si.size();

}

void get_relative_out_degrees(Graph &g, SolutionRepresentation &sol) {
    set<int> indices;
    for (int i : sol.get_set_indices()) indices.insert(i);
    if (sol.book.b_add_node.last_add_operation == -1) {
        cout << "Precomputing relative_out_degrees\n";
        for (int i : sol.get_set_indices()) {
            sol.book.b_add_node.relative_out_degrees[i] = relative_out_degree(g, sol, i);
        }
    } else {
        set<int> modified_clusters = sol.book.modified_clusters.query(sol.book.b_add_node.last_add_operation, sol.book.operation_number - 1);
        for (int i : modified_clusters) {
            if (indices.find(i) != indices.end()) {
                sol.book.b_add_node.relative_out_degrees[i] = relative_out_degree(g, sol, i);
            }
        }   
    }
    //cout << "ok\n";
    sol.book.b_add_node.empty_pq();
    for (int i : sol.get_set_indices()) {
        sol.book.b_add_node.pq_relative_out_degrees.push(pair<int, int>(sol.book.b_add_node.relative_out_degrees[i], i));
    }
    //cout << "ok2\n";
}

int highest_relative_out_degree(Graph &g, SolutionRepresentation &sol) {
    //map<double, int> relative_out_degrees = get_relative_out_degrees(g, sol);
    //map<double, int>::reverse_iterator it = relative_out_degrees.rbegin();
    //return it->second;
    get_relative_out_degrees(g, sol);
    return sol.book.b_add_node.pq_relative_out_degrees.top().second;
}

int add_node_to_set_cost(Graph &g, SolutionRepresentation &sol, int si, set<int> &set_nodes, int v) {
    //edges we must delete before adding the node
    int edges_to_delete = 0;
    //edges we must add after adding the node
    int edges_to_add = 0;
    bool in_same_cluster = false;
    for (int i : set_nodes) {
        in_same_cluster = false;
        for (int w : sol.get_node_to_clusters(v)) {
            if (w == si) continue;
            set<int> w_cluster = sol.get_set(w);
            if (w_cluster.find(i) != w_cluster.end()) {
                //cout << v << " in same cluster as " << i << "\n";
                in_same_cluster = true;
                break;
            }
        }
        if (g.has_edge(i, v)) {
            if (!in_same_cluster) {
                edges_to_delete += g.get_edge_cost(i, v);
                //cout << "counting edge " << i << " " << v << "\n";
            }
        }
        //g does not have an edge between i and v
        else {
            if (!in_same_cluster) edges_to_add += g.get_edge_cost(i, v);
            
        }
    } 
    //+1 since we split the node by adding it
    //cout << "edges to add: " << edges_to_add << "\n";
    //cout << "edges to delete " << edges_to_delete << "\n";
    return edges_to_add - edges_to_delete + 1;
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
                results[add_node_to_set_cost(g, sol, si, set_nodes, w)] = w;
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

int add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book) {
    int si = highest_relative_out_degree(g, sol);
    map<int, int> best_nodes = best_nodes_to_add(g, sol, si);
    sol.book.b_add_node.v = best_nodes.begin()->second;
    sol.book.b_add_node.si = si;
    //sol.add(best_nodes.begin()->second, si);
    //book.revert_add_node[0] = best_nodes.begin()->second;
    //book.revert_add_node[1] = si;
    //sol.print_solution();
    return best_nodes.begin()->first;
}
/*
//Possibly not working yet.
int weighted_random_add_node(Graph &g, SolutionRepresentation &sol, Bookkeep &book) {
    map<double, int> out_deg = get_relative_out_degrees(g, sol);
    int ind = weighted_random_index(10, out_deg.size(), 1.7);
    
    int counter = 0;
    map<double, int>::iterator it = out_deg.begin();
    while (counter != ind) {
        it++;
        counter += 1;
    }
    int si = it->second;

    map<int, int> best_nodes = best_nodes_to_add(g, sol, si);
    ind = weighted_random_index(2, out_deg.size(), 3);

    counter = 0;
    map<int, int>::iterator it2 = best_nodes.begin();
    while (counter != ind) {
        it2++;
        counter += 1;
    }

    sol.book.b_add_node.v = it->second;
    sol.book.b_add_node.si = si;

    return it->first;
    //book.revert_add_node[0] = it2->second;
    //book.revert_add_node[1] = si;
}
**/