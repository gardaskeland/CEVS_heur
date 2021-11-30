
#include "read_sol.h"


SolutionRepresentation read_sol_file(string s) {
    ifstream myfile(s);
    string line;
    getline(myfile, line);
    istringstream iss(line);
    string next;

    int num_sets;
    iss >> num_sets;

    SolutionRepresentation sr;

    int u;
    for (int i = 0; i < num_sets; i++) {
        set<int> s;
        getline(myfile, line);
        istringstream iss(line);
        while(!(iss.rdbuf() -> in_avail() == 0)) {
            iss >> u;
            s.insert(u - 1);
        }
        sr.add_set(s);
    }

    return sr;
}