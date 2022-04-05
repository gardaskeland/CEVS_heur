#include "find_kernel.h"
#include "revert_kernel.h"
#include "../CEVS/read_file.h"
#include "../CEVS/utility/shallow_solution.h"
//#include "operators/add_node.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <chrono>


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

void test_reduction() {
    ostringstream oss;
    //oss.clear();
    //oss.str(string());
    //oss << "../../../heur/heur" << integer_to_three_digits(i) << ".gr";
    string filename = "../CEVStest/test_graphs/g12.txt";
    cout << "Working on file " << filename << "\n";
    vector<vector<int>> adj = read_gz_file(filename);
    int num_edges = 0;
    for (vector<int> vec: adj) {
        num_edges += vec.size();
    }
    Graph g = Graph(adj);
    cout << "Number of nodes and edges in input graph: " << g.n << ", " << num_edges << "\n";
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    cout << "print graph wg: \n";
    for (int i = 0; i < wg.n; i++) {
        cout << "N(" << i << "):";
        for (int v : wg.adj[i]) cout << v << " ";
        cout << "\n";
    }
    cout << "Number of nodes and edges of critical clique graph: " << wg.n << ", " << 2*revert.num_edges << "\n";

    cout << "sets removed in reduction:\n";
    for (set<int> s : revert.removed_sets) {
        for (int i : s) cout << i << " ";
        cout << "\n";
    } 
    cout << "\n";
    cout << "sets isolated:\n";
    for(set<int> s : revert.isolated_cc) {
        for(int i : s) cout << i << " ";
        cout << "\n";
    }
    cout << "\n";
    cout << "other sets:\n";
    for (set<int> s : revert.other_cc) {
        for(int i : s) cout << i << " ";
        cout << "\n";
    }

    ShallowSolution shallow;
    for (int u = 0; u < wg.n; u++) {
        shallow.node_in_clusters[u] = set<int>();
    }
    set<int> new_set;
    for (int u = 0; u < wg.n; u++) {
        new_set = {u};
        shallow.clusters[u] = move(new_set);
        shallow.node_in_clusters[u].insert(u);
    }

    ShallowSolution reverted = from_cc_sol_to_sol(g, shallow, revert);

    cout << reverted.solution_as_string() << "\n";

    cout << "node_in_clusters:\n";
    for (auto it = reverted.node_in_clusters.begin(); it != reverted.node_in_clusters.end(); it++) {
        cout << it->first << ": ";
        for (int i : it->second) cout << i << " ";
        cout << "\n";
    }

}


int main() {

    //test_reduction();
    //exit(0);
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    ostringstream oss;
    string filename;
    ofstream write_results;
    write_results.open("percent_reduction.txt");
    write_results << "filename, percent nodes, percent edges\n";
    double sum_percent_nodes = 0;
    double sum_percent_edges = 0;
    double best_percentage_nodes = 0;
    double best_percentage_edges = 0;
    int num_graphs = 3000;
    for (int i = 1; i < num_graphs*2; i = i + 2) {
        oss.clear();
        oss.str(string());
        oss << "../../../heur/heur" << integer_to_three_digits(i) << ".gr";
        filename = oss.str();
        cout << "Working on file " << filename << "\n";
        vector<vector<int>> adj = read_gz_file(filename);
        int num_edges = 0;
        for (vector<int> vec: adj) {
            num_edges += vec.size();
        }
        Graph g = Graph(adj);
        cout << "Number of nodes and edges in input graph: " << g.n << ", " << num_edges << "\n";
        RevertKernel revert;
        WeightedGraph wg = find_critical_clique_graph(g, revert);
        cout << "Number of nodes and edges of critical clique graph: " << wg.n << ", " << 2*revert.num_edges << "\n";

        double percent_reduction_nodes = (100 * (g.n - wg.n)) / g.n;
        if (percent_reduction_nodes > best_percentage_nodes) best_percentage_nodes = percent_reduction_nodes;
        double percent_reduction_edges = (100 * (num_edges - revert.num_edges)) / num_edges;
        if (percent_reduction_edges > best_percentage_edges) best_percentage_edges = percent_reduction_edges;
        sum_percent_nodes += percent_reduction_nodes;
        sum_percent_edges += percent_reduction_edges;
        write_results << filename << ": " << percent_reduction_nodes << ", " << percent_reduction_edges << "\n";
    }
    write_results << "average percent reduction on nodes: " << sum_percent_nodes / num_graphs << "\n";
    write_results << "average percent reduction on edges: " << sum_percent_edges / num_graphs << "\n";
    write_results << "best percentage reduction on nodes: " << best_percentage_nodes << "\n";
    write_results << "best percentage reduction on edges " << best_percentage_edges << "\n";
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    cout << "time elapsed: " << time_elapsed / 1000000 << " seconds \n";
    write_results << "time elapsed: " << time_elapsed / 1000000 << " seconds \n";
    write_results.close();
}

