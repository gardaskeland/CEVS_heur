#include "test_greedy_merge.h"

void test_merge_cost() {
    string filename = "../CEVStest/test_graphs/g1.txt";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g = Graph(adj);

    filename = "test_sol_rep/g1_sol_1.txt";
    SolutionRepresentation sr = read_sol_file(filename);

    int res = merge_cost(sr, g, 0, 1);
    if (res != -2) {
        cout << "FAIL test_merge_cost: Wrong result for g1: " << res << "\n";
    }


    filename = "../CEVStest/test_graphs/g2.txt";
    adj = read_gz_file(filename);
    g = Graph(adj);

    filename = "test_sol_rep/g2_sol_1.txt";
    SolutionRepresentation sr2 = read_sol_file(filename);

    res = merge_cost(sr2, g, 0, 1);
    if (res != 4) {
        cout << "FAIL merge_test_cost: wrong value for g2: " << res << "\n";
    }


    filename = "../CEVStest/test_graphs/g3.txt";
    adj = read_gz_file(filename);
    g = Graph(adj);

    filename = "test_sol_rep/g3_sol_1.txt";
    SolutionRepresentation sr3 = read_sol_file(filename);

    res = merge_cost(sr3, g, 1, 3);
    if (res != -1) {
        cout << "FAIL merge_test_cost: wrong value for g3: " << res << "\n";
    }

    cout << "test_merge_cost finished\n";
}

void run_tests_greedy_merge() {
    cout << "start test merge_cost\n";

    test_merge_cost();
        
    cout << "run_tests_greedy_merge finished\n";    
}

