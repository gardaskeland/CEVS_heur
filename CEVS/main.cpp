
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
    int num_operations = 10;
    for (int i = 1; i < 2; i = i + 2) {
        oss.clear();
        oss.str(string());
        oss << "../../../heur/heur" << integer_to_three_digits(i) << ".gr";
        filename = oss.str();
        cout << "Working on file " << filename << "\n";
        vector<vector<int>> adj = read_gz_file(filename);
        Graph g(adj);
        ShallowSolution sol = local_search(g, num_operations);
        cout << "Best solution:\n";
        SolutionRepresentation calculate_sol = SolutionRepresentation(num_operations);
        map<int, set<int>> clusters = sol.get_clusters();
        for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
            calculate_sol.add_set(it->second);
        }
        calculate_sol.print_solution();
        cout << "Cost of solution: " << calculate_sol.cost_solution(g) << "\n"; 
    }

}