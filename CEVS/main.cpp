
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "read_file.h"
#include "graph.h"
#include "metaheuristics/local_search.h"
#include "utility/shallow_solution.h"
#include "bookkeep/b_merge.h"
#include <chrono>
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
    int num_operations = 2000;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (int i = 3; i < 8; i = i + 2) {
        oss.clear();
        oss.str(string());
        oss << "../../../heur/heur" << integer_to_three_digits(i) << ".gr";
        filename = oss.str(); // "../CEVStest/test_graphs/g8.txt";
        cout << "Working on file " << filename << "\n";
        vector<vector<int>> adj = read_gz_file(filename);
        Graph g = Graph(adj);
        ShallowSolution sol = local_search_on_cc(g, num_operations);
        cout << "Best solution:\n";
        SolutionRepresentation calculate_sol = SolutionRepresentation(num_operations);
        map<int, set<int>> clusters = sol.get_clusters();
        for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
            calculate_sol.add_set(it->second);
        }
        calculate_sol.print_solution();
        cout << "Cost of solution: " << calculate_sol.cost_solution(g) << "\n"; 
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    cout << "time used in seconds: " << time_elapsed / 1000000 << "\n";

}