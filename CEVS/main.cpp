
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "read_file.h"
#include "graph.h"
#include "metaheuristics/local_search.h"
using namespace std;

int main() {
    string filename = "../../../heur/heur001.gr";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g(adj);
    SolutionRepresentation sol = local_search(g);
    cout << "Best solution:\n";
    sol.print_solution();
    cout << "Cost of solution: " << sol.cost_solution(g) << "\n";
}