#ifndef SHALLOW_SOLUTION_H
#define SHALLOW_SOLUTION_H

#include <set>
#include <map>

using namespace std;

class ShallowSolution {
    public:
        map<int, set<int>> clusters;
        map<int, set<int>> node_in_clusters;

    ShallowSolution(map<int, set<int>> clusters_sol, map<int, set<int>> node_in_clusters_sol) {
        clusters = clusters_sol;
        node_in_clusters = node_in_clusters_sol;
    }

    ShallowSolution() {

    }

    map<int, set<int>> get_clusters();

};


#endif