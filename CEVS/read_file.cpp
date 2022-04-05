
#include "read_file.h"


vector<vector<int>> read_gz_file(string s) {
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

vector<vector<int>> read_csv_graph(string &filename, int num_nodes) {
    vector<int> row;
    string line, word;
    vector<vector<int>> adj_list(num_nodes, vector<int>());

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

vector<vector<int>> read_gml(string &filename) {
    vector<vector<int>> adj;
    string line, word;
    fstream file(filename, ios::in);
    stringstream str("a");
    int src, target;
    while (true) {
        getline(file, line);
        //cout << "line: " << line << "\n";
        if (line.empty()) break;
        str.clear();
        str = stringstream(line);
        getline(str, word, ' ');
        while(word.empty()) getline(str, word, ' ');
        //cout << "s:" << word << ":e\n";
        if (word.compare("graph") == 0) continue;
        if (word.compare("node") == 0) {
            //cout << "read node \n";
            getline(file, line);
            getline(file, line);
            getline(file, line);
            getline(file, line);
            adj.emplace_back(vector<int>());
            continue;
        }
        else if (word.compare("edge") == 0) {
            //cout << "read edge\n";
            getline(file, line);
            //cout << "line: :" << line << ":\n";
            str.clear();
            str = stringstream(line);
            getline(str, word, ' ');
            while(word.empty()) getline(str, word, ' ');
            getline(str, word, ' ');
            //cout << "word: :" << word << ":\n";
            src = stoi(word);

            getline(file, line);
            str.clear();
            str = stringstream(line);
            getline(str, word, ' ');
            while(word.empty()) getline(str, word, ' ');
            getline(str, word, ' ');
            target = stoi(word);
            getline(file, line);
            getline(file, line);
            getline(file, line);

            adj[src].push_back(target);
            adj[target].push_back(src);
        }
    }
    return adj;
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

map<int, set<int>> read_csv_groups_football(string &filename) {
    string line, word;
    vector<string> row;
    map<int, set<int>> to_return;
    fstream file(filename, ios::in);
    getline(file, line);
    while(getline(file, line)) {
        row.clear();

        stringstream str(line);

        while(getline(str, word, ','))
            row.push_back(word);
        to_return[stoi(row[2])].insert(stoi(row[0]));
        
    }

    file.close();

    return to_return;
}

map<int, set<int>> read_solution(string &filename) {
    string line, word;
    vector<string> row;
    map<int, set<int>> to_return;
    fstream file(filename, ios::in);
    getline(file, line);
    set<int> to_insert;
    int counter = 0;
    while(getline(file, line)) {
        //cout << line << "\n";
        to_insert = set<int>();
        stringstream str(line);
        while(getline(str, word, ',')) {
            to_insert.insert(stoi(word));
        }
        to_return[counter++] = to_insert;
    }
    return to_return;
}