#include "graph_.h"
#include "functions.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include "../CEVS/graph.h"
#include "../CEVS/solution_representation.h"

using namespace std;

vector<vector<int>> read_gz_file(string s) {
    ifstream myfile(s);
    string line;
    getline(myfile, line);
    istringstream iss(line);
    string next;
    iss >> next;
    iss >> next;

    int nodes;
    int edges;
    iss >> nodes;
    iss >> edges;
    //cout << "nodes: " << nodes << ", edges: " << edges << "\n";

    int u, v;
    vector<vector<int>> adj(nodes);
    for (int i = 0; i < edges; i++) {
        getline(myfile, line);
        istringstream iss(line);
        iss >> u; iss >> v;
        adj[u - 1].push_back(v - 1);
        adj[v - 1].push_back(u - 1);
    }
    for (int i = 0; i < adj.size(); i++) {
        sort(adj[i].begin(), adj[i].end());
    }

    /**cout << "Edges of graph: \n";
    for (int i = 0; i < nodes; i++) {
        for (int j : adj[i]) {
            //cout << j << " ";
        }
        //cout << "\n";
    }
    */
    return adj;
}

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

int main() {
    ostringstream oss;
    string filename;
    vector<vector<int>> adj_lst;
    int cost;
    vector<set<int>> components;
    for (int i = 3; i < 4; i = i + 2) {
        oss.clear();
        oss.str(string());
        oss << "../../../heur/heur" << integer_to_three_digits(i) << ".gr";
        filename = oss.str();
        cout << "Working on file " << filename << "\n";
        adj_lst = read_gz_file(filename);
        Graph_ g = Graph_(adj_lst.size());
        Graph g_initial = Graph(adj_lst);
        g.adj = adj_lst;
        g_initial.adj = adj_lst;
        //g.print_graph();

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        components = greedy_heuristic(g);
        cout << "After heuristic: \n";
        SolutionRepresentation sol = SolutionRepresentation(g_initial.n, 1);

        for (set<int> s : components) {
            sol.add_set(s);
        }

        cost = sol.cost_solution(g_initial);
        cout << "Solution: \n";
        sol.print_solution();
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        //g.print_graph();
        cout << "Cost: " << cost << "\n";
        double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        cout << "time taken: " << time_elapsed / 1000000 << "\n";
    }
}

void test() {
    Graph_ g_ = Graph_(5);
    g_.add_edge(0, 1);
    g_.print_graph();
    g_.add_edge(2, 3);
    g_.print_graph();
    g_.add_edge(3, 4);
    g_.print_graph();
    g_.add_edge(1, 3);
    g_.print_graph();
    g_.add_edge(2, 4);
    g_.print_graph();
    g_.remove_edge(2, 4);
    g_.print_graph();
    g_.split_vertex_(0, 1, 3);
    g_.print_graph();

    vector<vector<int>> adj_lst = read_gz_file("../CEVS/exact/g2.txt");
    Graph_ g = Graph_(adj_lst.size());
    g.adj = adj_lst;
    g.print_graph();
    g.split_vertex_(0, 4, 3);
    g.print_graph();

    adj_lst = read_gz_file("../CEVS/exact/g9.txt");
    g = Graph_(adj_lst.size());
    g.adj = adj_lst;
    g.print_graph();
    g.split_vertex_(2, 3, 7);
    g.print_graph();
    g.split_vertex_(9, 7, 4);
    g.print_graph();
}