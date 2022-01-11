#include "shallow_solution.h"

map<int, set<int>> ShallowSolution::get_clusters() {
    return clusters;
}

void ShallowSolution::print_solution() {
    cout << "[";
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); ++it) {
        cout << "[" << it->first << ": ";
        for (int i : it->second) {
            cout << i << ", ";
        }
        cout << "], ";
    }
    cout << "]\n";
}