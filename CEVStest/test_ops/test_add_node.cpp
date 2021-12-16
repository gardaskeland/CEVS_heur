#include "test_add_node.h"


void test_relative_out_degree() {
    string filename = "../CEVStest/test_graphs/g1.txt";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g = Graph(adj);

    filename = "test_sol_rep/g1_sol_2.txt";
    SolutionRepresentation sr = read_sol_file(filename);

    int res = relative_out_degree(g, sr, 0);
    if (res != 2) {
        cout << "FAIL test_relative_out_degree: Wrong result for g1 set 0: " << res << "\n";
    }
    res = relative_out_degree(g, sr, 1);
    if (res != 1) {
        cout << "FAIL test_relative_out_degree: Wrong result for g1 set 1: " << res << "\n";
    }

    filename = "../CEVStest/test_graphs/g5.txt";
    adj = read_gz_file(filename);
    g = Graph(adj);

    filename = "test_sol_rep/g5_sol_1.txt";
    sr = read_sol_file(filename);

    double res_ = relative_out_degree(g, sr, 4);
    if (res_ != 3) {
        cout << "FAIL test_relative_out_degree: Wrong result for g5: " << res_ << "\n";
    }
}

void test_highest_relative_out_degree() {
    vector<vector<int>> adj = read_gz_file("../CEVStest/test_graphs/g5.txt");
    Graph g = Graph(adj);

    SolutionRepresentation sr = read_sol_file("test_sol_rep/g5_sol_1.txt");

    double res_ = highest_relative_out_degree(g, sr);
    if (res_ != 4) {
        cout << "FAIL test_highest_relative_out_degree: Wrong result for g5: " << res_ << "\n";
    }
}

void test_best_nodes_to_add() {
    vector<vector<int>> adj = read_gz_file("../CEVStest/test_graphs/g6.txt");
    Graph g = Graph(adj);

    SolutionRepresentation sr = read_sol_file("test_sol_rep/g6_sol_1.txt");

    vector<pair<int, int>> best_nodes = best_nodes_to_add(g, sr, 1);
    int res = best_nodes.begin()->second;
    if (res != 2) {
        cout << "FAIL test_best_nodes_to_add Wrong result for g6: " << res << "\n";
    }
}

void run_tests_add_node() {
    cout << "run_tests_add_node\n";
    test_relative_out_degree();
    test_highest_relative_out_degree();
    test_best_nodes_to_add();
}