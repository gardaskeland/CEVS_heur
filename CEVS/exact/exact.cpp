#include "../solution_representation.h"
#include "../graph.h"
#include "../read_file.h"
#include <cmath>
#include <vector>
#include <set>

string integer_to_three_digits(int i) {
    ostringstream oss;
    if (i >= 100) {
        oss << i;
        return oss.str();
    }
    else if (i >= 10) {
        oss << 0 << i;
        return oss.str();
    } else {
        oss << 0 << 0 << i;
        return oss.str();
    }
}

int cost_operations(Graph &g, vector<set<int>> &clusters, vector<set<int>> &node_in_clusters) {
    set<pair<int, int>> edge_deletions;
    set<pair<int, int>> edge_additions;
    int delete_counter = 0;
    int add_counter = 0;
    int vs_counter = 0;
    map<int, set<int>> node_in_cluster_with;

    //Iteate over all nodes and store which nodes they are supposed to be in a cluster with.
    set<int> temp;
    for (int i = 0; i < g.n; i++) {
        temp = set<int>();
        for (set<int>::iterator it = node_in_clusters[i].begin(); it != node_in_clusters[i].end(); ++it) {
            temp.insert(clusters[*it].begin(), clusters[*it].end());
        }
        node_in_cluster_with[i] = temp;
    }
    /**
    for (int i = 0; i < g.n; i++) {
        cout << "node " << i << " in clusters with ";
        print_integer_set(node_in_cluster_with[i]);
        cout << "\n";
    }
    */

    //edge deletions: For each node v in N(u), is v in the same cluster?
    pair<int, int> edge;
    pair<int, int> opposite;
    for (int i = 0; i < g.n; i++) {
        for (int v : g.adj[i]) {
            if (node_in_cluster_with[i].find(v) != node_in_cluster_with[i].end()) {
                continue;
            } else {
                edge = pair<int, int>(i, v);
                if (edge_deletions.find(edge) != edge_deletions.end()) {
                    continue;
                } else {
                    opposite = pair<int, int>(v, i);
                    edge_deletions.insert(edge);
                    edge_deletions.insert(opposite);
                    delete_counter += g.get_edge_cost(i, v);
                    //cout << "edge cost of " << i << " " << v << ": " << g.get_edge_cost(i, v) << "\n";
                }
            }
        }
    }

    //edge additions: For each node v in the same cluster as u, does it have and edge?
    for (int i = 0; i < g.n; i++) {
        for (set<int>::iterator it = node_in_cluster_with[i].begin();
         it != node_in_cluster_with[i].end(); ++it) {
             if (i == *it) {
                 continue;
             }
             if (g.has_edge(i, *it)) {
                 continue;
             } else {
                 edge = pair<int, int>(i, *it);
                 if (edge_additions.find(edge) != edge_additions.end()) {
                     continue;
                 } else {
                     opposite = pair<int, int>(*it, i);
                     edge_additions.insert(edge);
                     edge_additions.insert(opposite);
                     add_counter += g.get_edge_cost(i, *it);
                     //cout << "edge cost of " << i << " " << *it << ": " << g.get_edge_cost(i, *it) << "\n";
                 }
             }
         } 
    }

    //vertex splitting: For each node, add the number of clusters it is in -1 to the counter.
    int sz;
    for (int i = 0; i < g.n; i++) {
        sz = node_in_clusters[i].size();
        if (sz > 0) {
            vs_counter += g.get_node_weight(i) * (sz - 1);
            //cout << "g.get_node_weight for " << i << ": " << g.get_node_weight(i) << "\n";
        }
    }

    //For debug
    //printEdgeChanges(edge_deletions, edge_additions);

    //tuple<int, int, int> result = tuple<int, int, int>(delete_counter, add_counter, vs_counter);
    return delete_counter + add_counter + vs_counter;
}

void recursion(Graph &g, vector<int> &set_stack, int &best_cost_p, vector<set<int>> &best_solution_p, int p, int last) {
    //cout << "p, last: " << p << ", " << last << "\n";
    if (p == 0) {
        int check_feasibility = 0;
        for (int i : set_stack) {
            check_feasibility = check_feasibility | i;
        }
        if (check_feasibility != pow(2, g.n) - 1) return;
        vector<set<int>> clusters;
        set<int> to_insert;
        for (int i : set_stack) {
            to_insert.clear();
            for (int j = 0; j < g.n; j++) {
                if ((i & (1 << j) ^ 0) != 0) {
                    to_insert.insert(j);
                } 
            }
            clusters.push_back(to_insert);
        }

        vector<set<int>> node_in_clusters(g.n, set<int>());
        for (int si = 0; si < clusters.size(); si++) {
            for (int u : clusters[si]) {
                node_in_clusters[u].insert(si);
            }
        }

        int cost = cost_operations(g, clusters, node_in_clusters);

        if (cost < best_cost_p) {
            best_cost_p = cost;
            best_solution_p = clusters;
        }
        return;
    }
    for (int i = last + 1; i < pow(2, g.n); i++) {
        set_stack.push_back(i);
        recursion(g, set_stack, best_cost_p, best_solution_p, p-1, i);
        set_stack.pop_back();
    }
    
}

void print_solution(vector<set<int>> clusters) {
    cout << "[";
    for (int i = 0; i < clusters.size(); i++) {
        cout << "[" << i << ": ";
        for (int j : clusters[i]) {
            cout << j << ", ";
        }
        cout << "], ";
    }
    cout << "]\n";
}

int main() {
    ostringstream oss;
    string filename;
    oss.clear();
    oss.str(string());
    oss << "g9.txt"; //"../../../../heur/heur" << integer_to_three_digits(1) << ".gr";
    filename = oss.str(); // "../CEVStest/test_graphs/g8.txt";
    oss.clear();
    cout << "Working on file " << filename << "\n";
    vector<vector<int>> adj = read_gz_file(filename);
    //cout << "Done\n";
    Graph g = Graph(adj);
    SolutionRepresentation sol(g.n, 1); //g.n, 1
    vector<int> best_cost;
    int best_cost_p;
    vector<set<int>> best_solution_p;
    vector<set<int>> best_solution;
    sol.initial_solution_complete_graph(g.n);
    int cost_complete_graph = sol.cost_solution(g);
    int best_cost_overall = cost_complete_graph;
    best_cost.push_back(best_cost_overall);
    vector<int> set_stack;
    cout << "Cost of complete graph: " << cost_complete_graph << "\n\n----------\n";
    //p is number of sets in solution.
    for (int p = 2; p < g.n; p++) {
        set_stack.empty();
        best_cost_p = 10000000;
        cout << "start recursion for " << p << "\n";
        recursion(g, set_stack, best_cost_p, best_solution_p, p, 0);
        cout << "recursion done for " << p << "\n";
        if (best_cost_p <= best_cost_overall) {
            best_cost_overall = best_cost_p;
            best_solution = best_solution_p;
        } else {
            
        }
        cout << "Best solution with size " << p << ":\n";
        print_solution(best_solution);
        cout << "Cost of solution: " << best_cost_p << "\n";
        cout <<"\n------\n";
    }
    cout << "Cost of best solution: " << best_cost_overall << "\n";
    cout << "Best solution: \n";
    print_solution(best_solution);
    cout << "Sets: " << best_solution.size() << "\n";

}
