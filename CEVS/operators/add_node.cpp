#include "add_node.h"

double relative_out_degree(Graph g, SolutionRepresentation sol, int si) {
    int count_out_edges = 0;
    set<int> nodes_in_si = sol.get_set(si);
    //cout << "Start new test case\n";
    for (int node : nodes_in_si) {
        vector<int> neighbours = g.adj[node];
        for (int neighbour : neighbours) {
            if (nodes_in_si.find(neighbour) != nodes_in_si.end()) {
                continue;
            }
            else {
                //cout << "count node " << neighbour << "\n";
                count_out_edges += 1;
            }
        }
    }
    return count_out_edges / nodes_in_si.size();

}