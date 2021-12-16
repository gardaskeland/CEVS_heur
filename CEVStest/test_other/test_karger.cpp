#include "test_karger.h"

void test_karger (){
    string filename = "../CEVStest/test_graphs/g9.txt";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g = Graph(adj);

    filename = "test_sol_rep/g9_sol_1.txt";
    SolutionRepresentation sr = read_sol_file(filename);

    int index = 0;
    pair<set<int>, set<int>> p = find_min_cut(g, sr, index);

    if (p.first.size() + p.second.size() != 7) {
        cout << "FAIL test_karger: g9 was not split correctly since the sum of sizes of sets is wrong: " << \
            p.first.size() + p.second.size() << "\n";
    }

    for (int i = 0; i < 4; i++) {
        //cout << i << " ";
        if (!(p.first.find(i) != p.first.end())) {
            cout << "FAIL test_karger g9: first component does not contain " << i << "\n";
        }
    }
    //cout << "\n";
    for (int i = 4; i < 7; i++) {
        //cout << i << " ";
        if (!(p.second.find(i) != p.second.end())) {
            cout << "FAIL test_karger g9: second component does not contain " << i << "\n";
        }
    }
    //cout << "\n";
    
    filename = "../CEVStest/test_graphs/g10.txt";
    adj = read_gz_file(filename);
    g = Graph(adj);

    filename = "test_sol_rep/g10_sol_1.txt";
    sr = read_sol_file(filename);

    index = 0;
    p = find_min_cut(g, sr, index);
    if (p.first.size() + p.second.size() != 12) {
        cout << "FAIL test_karger: g10 was not split correctly since the sum of sizes of sets is wrong: " << \
            p.first.size() + p.second.size() << "\n";
    }

    for (int i = 1; i < 7; i++) {
        if (!(p.first.find(i) != p.first.end())) {
            cout << "FAIL test_karger g10: first component does not contain " << i << "\n";
        }
    }
    for (int i = 7; i < 11; i++) {
        if (!(p.second.find(i) != p.second.end())) {
            cout << "FAIL test_karger g10: second component does not contain " << i << "\n";
        }
    }
}

void run_tests_karger() {
    cout << "Run tests for karger\n";
    test_karger();
}