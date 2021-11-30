#include "test_add_node.h"


void test_relative_out_degree() {
    string filename = "../CEVStest/test_graphs/g1.txt";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g = Graph(adj);

    filename = "test_sol_rep/g1_sol_2.txt";
    SolutionRepresentation sr = read_sol_file(filename);

    int res = relative_out_degree(g, sr, 0);
    if (res != 2) {
        cout << "FAIL test_merge_cost: Wrong result for g1 set 0: " << res << "\n";
    }
    res = relative_out_degree(g, sr, 1);
    if (res != 1) {
        cout << "FAIL test_merge_cost: Wrong result for g1 set 1: " << res << "\n";
    }

    filename = "../CEVStest/test_graphs/g5.txt";
    adj = read_gz_file(filename);
    g = Graph(adj);

    filename = "test_sol_rep/g5_sol_1.txt";
    sr = read_sol_file(filename);

    double res_ = relative_out_degree(g, sr, 4);
    if (res_ != 3) {
        cout << "FAIL test_merge_cost: Wrong result for g5: " << res_ << "\n";
    }
}

void run_tests_add_node() {
    cout << "run_tests_add_node\n";
    test_relative_out_degree();
}