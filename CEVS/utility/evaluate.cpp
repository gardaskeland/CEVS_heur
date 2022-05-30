#include "evaluate.h"

void add_to_map(map<int, set<int>> &map_, int si, int u) {
    if (!(map_.find(si) != map_.end())) map_[si] = set<int>();
    map_[si].insert(u);
}

map<int, set<int>> read_ground_truth_gml(string ground_truth) {
    //cout << "reading " << ground_truth << "\n";
    vector<vector<int>> adj;
    string line, word;
    fstream file(ground_truth, ios::in);
    stringstream str("a");
    map<int, set<int>> map_;
    int node_counter = 0;
    
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
            str = stringstream(line);
            getline(str, word, ' ');
            while(word.empty()) getline(str, word, ' ');
            bool skip_number = false;
            while(str) {
                getline(str, word, ' ');
                //cout << word << "\n";
                char word_s = word[word.size() - 2];
                if (word_s == ']') {
                    break;
                }
                if (skip_number) {
                    skip_number = false;
                    continue;
                }
                int start = 0;
                while(!isdigit(word[start])) start++;
                int end = start;
                while(isdigit(word[end])) end++;
                string numstr = word.substr(start, end - start);
                //cout << numstr << "\n";
                int si = stoi(numstr);
                add_to_map(map_, si, node_counter);
                skip_number = true;
            }
            getline(file, line);
            adj.emplace_back(vector<int>());
            node_counter++;
        }
        else if (word.compare("edge") == 0) break;
    }
    return map_;
}

void write_ground_truth_gml(map<int, set<int>> &truth) {
    ofstream out_file;
    string out_filename = "utility/Overlapping-NMI-master/ground_truth.txt";
    out_file.open(out_filename);
    for (auto it = truth.begin(); it != truth.end(); it++) {
        for (int i : it->second) out_file << i << " ";
        out_file << "\n";
    }
    out_file.close();
} 

void write_solution(SolutionRepresentation &sol) {
    ofstream out_file;
    string out_filename = "utility/Overlapping-NMI-master/solution.txt";
    out_file.open(out_filename);
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        for (int i : it->second) out_file << i << " ";
        out_file << "\n";
    }
    out_file.close();
}

optional<int> max_element_index(vector<int> &vec) {
    int max_val = vec[0];
    int max_ind = 0;
    bool two_equal = false;
    for (int i = 1; i < vec.size(); i++) {
        if (vec[i] > max_val) {
            max_val = vec[i];
            max_ind = i;
            two_equal = false;
        } else if (vec[i] == max_val) {
            two_equal = true;
        }
    }
    //cout << "two_equal: " << two_equal << "\n";
    if (two_equal) {
        return optional<int>();
    } else {
        return optional<int>(max_ind);
    }
}

pair<double, double> find_majority_accuracy(Graph &g, SolutionRepresentation &sol, map<int, set<int>> &ground_truth) {
    vector<bool> marked(g.n, false);
    vector<bool> wrong_set(g.n, false);
    vector<int> membership;
    optional<int> max_element;
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        membership = vector<int>(ground_truth.size(), 0);
        for (int node : it->second) {
            for (auto it2 = ground_truth.begin(); it2 != ground_truth.end(); it2++) {
                set<int> s = it2->second;
                if (s.find(node) != s.end()) membership[it2->first] += 1;
            }
        }
        max_element = max_element_index(membership);
        if (!max_element.has_value()) {
            //for (int node : it->second) {
            //    wrong_set[node] = true;
            //}
            continue;
        }
        for (int node : it->second) {
            set<int> majority_set = ground_truth[max_element.value()];
            if (majority_set.find(node) != majority_set.end()) {
                marked[node] = true;
            } else {
                wrong_set[node] = true;
            }
        }
    }
    vector<int> correctly_classified;
    copy_if(marked.begin(), marked.end(), back_inserter(correctly_classified), [](bool b){ return b == true; });

    vector<int> incorrectly_classified;
    copy_if(wrong_set.begin(), wrong_set.end(), back_inserter(incorrectly_classified), [](bool b){ return b == true; });

    pair<double,double> p((double)correctly_classified.size() / (double)g.n, (double)incorrectly_classified.size() / (double)g.n);

    return p;
}

vector<double> evaluate_alns(Graph &g, SolutionRepresentation &sol, string ground_truth) {

    map<int, set<int>> truth = read_ground_truth_gml(ground_truth);

    write_ground_truth_gml(truth);

    write_solution(sol);

    int status = system("./utility/Overlapping-NMI-master/onmi utility/Overlapping-NMI-master/solution.txt utility/Overlapping-NMI-master/ground_truth.txt -o \
        > utility/Overlapping-NMI-master/eval.txt");


    //Use https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po to read from cmd

    return vector<double>();
    
}