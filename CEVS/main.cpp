
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "read_file.h"
#include "graph.h"
#include "metaheuristics/local_search.h"
#include "metaheuristics/simulated_annealing.h"
#include "metaheuristics/local_search_lp.h"
#include "utility/shallow_solution.h"
#include "bookkeep/b_merge.h"
#include <chrono>
#include "operators/add_node.h"
using namespace std;

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


//This works now, but very slowly already for 120 vertices.
int main() {

    ostringstream oss;
    string filename;
    int num_operations = 20;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<double> time_elapsed_record;
    for (int i = 1; i < 20; i = i + 2) {
        oss.clear();
        oss.str(string());
        oss << "../../../heur/heur" << integer_to_three_digits(i) << ".gr";
        filename = oss.str(); // "../CEVStest/test_graphs/g8.txt";
        cout << "Working on file " << filename << "\n";
        vector<vector<int>> adj = read_gz_file(filename);
        Graph g = Graph(adj);
        for (int j = 1; j < 2; j++) {
            chrono::steady_clock::time_point begin_ = chrono::steady_clock::now();
            ShallowSolution sol = local_search_lp(g, num_operations);
            chrono::steady_clock::time_point end_ = chrono::steady_clock::now();
            cout << "Best solution:\n";
            double time_elapsed_ = chrono::duration_cast<chrono::microseconds>(end_ - begin_).count();
            time_elapsed_record.push_back(time_elapsed_);
            cout << "time used in seconds for graph " << filename << ": " << time_elapsed_ / 1000000 << "\n";
            SolutionRepresentation calculate_sol = SolutionRepresentation(g.n, num_operations);
            map<int, set<int>> clusters = sol.get_clusters();
            for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
                calculate_sol.add_set(it->second);
            }
            calculate_sol.print_solution();
            cout << "Solution feasible: " << calculate_sol.simple_feasibility_check() << "\n";
            cout << "Cost of solution: " << calculate_sol.cost_solution(g) << "\n";
            cout << "Number of splitting operations: " << calculate_sol.num_splits() << "\n";
            remove_nodes(g, calculate_sol);
            cout << "After using remove nodes: " << "\n";
            calculate_sol.print_solution();
            cout << "Solution feasible: " << calculate_sol.simple_feasibility_check() << "\n";
            cout << "Cost of solution: " << calculate_sol.cost_solution(g) << "\n";
            cout << "Number of splitting operations: " << calculate_sol.num_splits() << "\n";
            cout << "\n-----------------------------------------------\n\n";
        }
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    cout << "time used in seconds: " << time_elapsed / 1000000 << "\n";
    cout << "time elapsed for all instances: " << "\n";
    for (double t : time_elapsed_record) {
        cout << t << "\n";
    }

}