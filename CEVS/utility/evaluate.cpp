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

pair<double, double> read_eval() {
    string filename = "utility/Overlapping-NMI-master/eval.txt";
    string line, word;
    fstream file(filename, ios::in);
    stringstream str("a");

    getline(file, line);
    int start = 0;
    while(!isdigit(line[start])) start++;
    string num = line.substr(start);
    double omega = stod(num);

    getline(file, line);

    getline(file, line);
    start = 0;
    while(!isdigit(line[start])) start++;
    num = line.substr(start);
    double onmi = stod(num);

    return make_pair(omega, onmi);
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
        membership = vector<int>(ground_truth.rbegin()->first + 1, 0);
        for (int node : it->second) {
            for (auto it2 = ground_truth.begin(); it2 != ground_truth.end(); it2++) {
                //cout << it2->first << "\n";
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

//The function F from the paper "Extending the definition of modularity to directed graphs with overlapping communities"
double f_func(double a, double b) {
    //int p = 1;
    //double res = (double)1.0 / ((1.0 + exp(-2*p*a + p))*(1.0 + exp(-2*p*b + p)));
    //cout << res << "\n";
    //return res;
    //return std::max(a, b);
    if (a<=0.00001 || b<=0.00001) return 0.0;
    return (a+b)/2.0;
}

double extended_modularity(Graph &g, SolutionRepresentation &sol) {
    int m = g.num_edges;
    vector<double> alpha_i;
    for (int i = 0; i < g.n; i++) {
        alpha_i.push_back((double)1.0 / (double)sol.node_in_clusters[i].size());
    }

    vector<vector<double>> beta_out_c(g.n, vector<double>(sol.clusters.size()));
    //vector<vector<double>> beta_in_c(g.n, vector<double>(sol.clusters.size()));
    double result;
    int cluster_counter;
    for (int i = 0; i < g.n; i++) {
        cluster_counter = 0;
        for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
            result = 0.0;
            for (int j = 0; j < g.n; j++) {
                result += f_func(alpha_i[i] * ((it->second).find(i) != (it->second).end()), \
                    alpha_i[j]*((it->second).find(j) != (it->second).end()));
            }
            beta_out_c[i][cluster_counter] = result / (double)g.n;
            cluster_counter++;
        }
    }

    double modularity_sum = 0.0;
    cluster_counter = 0;
    double beta;
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        for (int u = 0; u < g.n - 1; u++) {
            for (int v = u+1; v < g.n; v++) {
                beta = f_func(alpha_i[u] * ((it->second).find(u) != (it->second).end()), \
                    alpha_i[v]*((it->second).find(v) != (it->second).end()));
                modularity_sum += beta*g.has_edge(u, v) - (beta_out_c[u][cluster_counter]*g.adj[u].size()* \
                    beta_out_c[v][cluster_counter]*g.adj[v].size()) / (2.0*(double)m);
                //cout << modularity_sum << endl;
            }
        }
        cluster_counter++;
    }

    return modularity_sum / ((double)m);
}

vector<double> evaluate_alns(Graph &g, SolutionRepresentation &sol, string ground_truth) {

    map<int, set<int>> truth = read_ground_truth_gml(ground_truth);

    write_ground_truth_gml(truth);

    write_solution(sol);

    int status = system("./utility/Overlapping-NMI-master/onmi utility/Overlapping-NMI-master/solution.txt utility/Overlapping-NMI-master/ground_truth.txt -o \
        > utility/Overlapping-NMI-master/eval.txt");
    status = system("./utility/Overlapping-NMI-master/onmi utility/Overlapping-NMI-master/solution.txt utility/Overlapping-NMI-master/ground_truth.txt -o");

    pair<double, double> values = read_eval();
    pair<double, double> accs = find_majority_accuracy(g, sol, truth);
    double eq = extended_modularity(g, sol);
    vector<double> to_return = {values.first, values.second, accs.first, accs.second, eq};

    return to_return;
    
}