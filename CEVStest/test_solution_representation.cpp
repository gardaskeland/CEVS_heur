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
    test_simple_feasibility_check();
    cout << "test_simple_feasibility_check finished\n";
    test_cost_operations();
}