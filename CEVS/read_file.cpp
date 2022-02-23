
#include "read_file.h"


vector<vector<int>> read_gz_file(string &s) {
    ifstream myfile(s);
    string line;
    getline(myfile, line);
    istringstream iss(line);
    string next;
    iss >> next;
    iss >> next;

    int nodes;
    int edges;
    iss >> nodes;
    iss >> edges;
    //cout << "nodes: " << nodes << ", edges: " << edges << "\n";

    int u, v;
    vector<vector<int>> adj(nodes);
    for (int i = 0; i < edges; i++) {
        getline(myfile, line);
        istringstream iss(line);
        iss >> u; iss >> v;
        adj[u - 1].push_back(v - 1);
        adj[v - 1].push_back(u - 1);
    }

    /**cout << "Edges of graph: \n";
    for (int i = 0; i < nodes; i++) {
        for (int j : adj[i]) {
            //cout << j << " ";
        }
        //cout << "\n";
    }
    */
    return adj;
}

vector<vector<int>> read_csv_graph(string &filename) {
    vector<int> row;
    string line, word;
    vector<vector<int>> adj_list(34, vector<int>());

    fstream file(filename, ios::in);
    getline(file, line);
    while(getline(file, line)) {
        row.clear();

        stringstream str(line);

        while(getline(str, word, ','))
            row.push_back(stoi(word));
        adj_list[row[0]].push_back(row[1]);
        adj_list[row[1]].push_back(row[0]);
    }

    file.close();

    return adj_list;
}

map<int, set<int>> read_csv_groups_karate(string &filename) {
    string line, word;
    vector<string> row;
    map<int, set<int>> to_return;
    to_return[0] = set<int>();
    to_return[1] = set<int>();
    fstream file(filename, ios::in);
    getline(file, line);
    while(getline(file, line)) {
        row.clear();

        stringstream str(line);

        while(getline(str, word, ','))
            row.push_back(word);
        to_return[stoi(row[2]) - 1].insert(stoi(row[0]));
        
    }

    file.close();

    return to_return;
}