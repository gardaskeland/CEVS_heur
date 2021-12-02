#include "test_solution_representation.h"
#include "../CEVS/solution_representation.h"
#include "../CEVS/read_file.h"
#include "read_sol.h"
#include <iostream>
#include <set>

using namespace std;

void test_add() {
    SolutionRepresentation sr;
    sr.initial_solution(10);
    sr.add(3, 5);
    set<int> s = sr.clusters[5];
    if (s.find(3) != s.end()) {

    } else {
        cout << "FAIL: 3 is not in set 5, add is not working";
    }
}

void test_add_set() {
    SolutionRepresentation sr;

    set<int> to_add;
    for (int i = 0; i < 7; i++) {
        to_add.insert(i);
    }
    sr.add_set(to_add);
    if (sr.clusters.size() != 1) {
        cout << "FAIL test_add_set: Set not added.\n";
    }
    if (sr.get_set(0).size() != 7) {
        cout << "FAIL test_add_set: Set added is of wrong size, size" << sr.get_set(0).size() << "\n";
        cout << "sr contains:\n";
        for (map<int,set<int>>::iterator it = sr.clusters.begin(); it != sr.clusters.end(); ++it) {
            for (int j : it->second) {
                cout << j << " ";
            }
            cout << "\n";
        }
    }

    set<int> to_add_2;
    for (int i = 1; i < 4; i++) {
        to_add_2.insert(i);
    }
    sr.add_set(to_add_2);
    if (sr.clusters.size() != 2) {
        cout << "FAIL test_add_set: Second set not added.\n";
    }
    if (sr.get_set(1).size() != 3) {
        cout << "FAIL test_add_set: Could not retrieve the correct second set.\n";
    }

    if (sr.get_node_to_clusters(1).size() != 2) {
        cout << "FAIL test_add_set: wrong number of elements in node_in_clusters[1]: "
        << sr.get_node_to_clusters(1).size() << "\n";
    }
    if (sr.get_node_to_clusters(6).size() != 1) {
        cout << "FAIL test_add_set: wrong number of elements in node_in_clusters[6]: "
        << sr.get_node_to_clusters(6).size() << "\n";
    }

    int thrd[] = {2, 3};
    set<int> third (thrd, thrd + 1);
    sr.add_set(third);

    if (sr.clusters.size() != 3) {
        cout << "FAIL test_add_set: Wrong number of clusters: " << sr.clusters.size() <<
            "instead of 3\n";
    }

    cout << "Finished test_add_set\n";
}

void test_remove_set() {
    SolutionRepresentation sr;

    set<int> to_add;
    for (int i = 0; i < 7; i++) {
        to_add.insert(i);
    }
    sr.add_set(to_add);

    sr.remove_set(0);

    if (sr.clusters.size() > 0) {
        cout << "FAIL test_remove_set: Still a set in sr after removal";
    }

    if (sr.get_node_to_clusters(1).size() > 0) {
        cout << "FAIL test_remove_set: Not all nodes removed from node_in_clusters";
    }

    cout << "Finished test_remove_set\n";
}

void test_simple_feasibility_check() {
    SolutionRepresentation sr;
    sr.initial_solution(10);
    if (!sr.simple_feasibility_check()) {
        cout << "FAIL: Initial solution is not feasible.";
    }
    sr.remove(8, 8);
    if (sr.simple_feasibility_check()) {
        cout << "FAIL: Solution without node 8 is said to be feasible.";
    }

}

void test_cost_operations() {

    vector<vector<int>> adj = read_gz_file("../CEVStest/test_graphs/g3.txt");
    Graph g = Graph(adj);
    SolutionRepresentation sr = read_sol_file("test_sol_rep/g3_sol_1.txt");

    tuple<int,int,int> tup = sr.cost_operations(g);

    if (get<0>(tup) != 2) {
        cout << "FAIL test_cost_operations for g3: Wrong number of edge deletions: " << get<0>(tup) << "\n";
    }
    if (get<1>(tup) != 0) {
         cout << "FAIL test_cost_operations for g3: Wrong number of edge additions: " << get<1>(tup) << "\n";
    }
    if (get<2>(tup) != 1) {
        cout << "FAIL test_cost_operations for g3: Wrong number of vertex splittings: " << get<2>(tup) << "\n";
    }


    adj = read_gz_file("../CEVStest/test_graphs/g4.txt");
    g = Graph(adj);
    sr = read_sol_file("test_sol_rep/g4_sol_1.txt");
    tup = sr.cost_operations(g);
    if (get<0>(tup) != 0) {
        cout << "FAIL test_cost_operations for g4: Wrong number of edge deletions: " << get<0>(tup) << "\n";
    }
    if (get<1>(tup) != 1) {
         cout << "FAIL test_cost_operations for g4: Wrong number of edge additions: " << get<1>(tup) << "\n";
    }
    if (get<2>(tup) != 2) {
        cout << "FAIL test_cost_operations for g4: Wrong number of vertex splittings: " << get<2>(tup) << "\n";
    }

    adj = read_gz_file("../CEVStest/test_graphs/g6.txt");
    g = Graph(adj);
    sr = read_sol_file("test_sol_rep/g6_sol_2.txt");
    tup = sr.cost_operations(g);
    if (get<0>(tup) != 5) {
        cout << "FAIL test_cost_operations for g6: Wrong number of edge deletions: " << get<0>(tup) << "\n";
    }
    if (get<1>(tup) != 16) {
         cout << "FAIL test_cost_operations for g6: Wrong number of edge additions: " << get<1>(tup) << "\n";
    }
    if (get<2>(tup) != 5) {
        cout << "FAIL test_cost_operations for g6: Wrong number of vertex splittings: " << get<2>(tup) << "\n";
    }
}

void run_tests_solution_representation() {
    test_add();
    cout << "test_add finished\n";
    test_add_set();
    test_simple_feasibility_check();
    test_remove_set();
    test_cost_operations();
    cout << "test_simple_feasibility_check finished\n";
    //test_cost_operations();
}