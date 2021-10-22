#include "test_solution_representation.h"
#include "../CEVS/solution_representation.h"
#include "../CEVS/read_file.h"
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
    string filename = "../heur/heur001.gr";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g = Graph(adj);
    SolutionRepresentation sr;
    sr.initial_solution(g.n);
    for (int i = 1; i < g.n; i++) {
        sr.remove(i, i);
    }
    sr.add(1, 0);
    sr.add(3, 0);
    sr.add(4, 0);
    sr.add(4, 1);
    sr.add(2, 1);
    sr.add(5, 1);
    sr.add(6, 2);
    sr.add(7, 2);
    sr.add(8, 2);
    sr.add(9, 2);
    sr.add(6, 3);
    sr.add(2, 3);
    tuple<int, int, int> cost = sr.cost_operations(g);
    cout << "Deletions: " << get<0>(cost) 
        << "\nAdditions: " << get<1>(cost)
        << "\nVertex splittings: " << get<2>(cost) << "\n";
}

void run_tests_solution_representation() {
    test_add();
    cout << "test_add finished\n";
    test_add_set();
    test_simple_feasibility_check();
    test_remove_set();
    cout << "test_simple_feasibility_check finished\n";
    //test_cost_operations();
}