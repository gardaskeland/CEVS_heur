
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "read_file.h"
#include "graph.h"
using namespace std;

int main() {
    string filename = "../../../heur/heur001.gr";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g(adj);
    cout << "hey";
    cout << "\n";
    vector<int> bla = g.adj[2];
    for(int j : bla) {
        cout << j << " ";
    }
}