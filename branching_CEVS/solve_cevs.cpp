#include "solve_cevs.h"

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

bool find_i(list<int> &li, int i) {
    bool yes = false;
    for (const auto& j : li) {
        //cout << "i: " << i << "\n";
        //cout << "j:" << j << "\n";
        if (j == i) {
            yes = true;
            break;
        }
    }
    return yes;
}

optional<vector<int>> find_p3(vector<list<int>> &adj, info_struct &info) {
    vector<vector<int>> parents(info.n_);
    for (int i = 0; i < info.n_; i++) parents[i] = vector<int>();
    //cout << "1\n";
    set<int> seen;
    vector<int> p3(3);
    queue<int> q;
    int n_size;
    int w;
    int min_, max_;
    for (int u = 0; u < info.n_; u++) {
        if (seen.find(u) != seen.end()) continue;
        //Also look at neighbours u
        n_size = adj[u].size();
        //cout << "2\n";
        vector<int> lst;
        for (int i : adj[u]) lst.push_back(i);
        for (int i = 0; i < n_size - 1; i++) {
            for (int j = i+1; j < n_size; j++) {
                //cout << "n_, i, j: " << info.n_ << " " << i << " " << j << "\n";
                if (find_i(adj[lst[i]], lst[j])) continue; //we have a 3-cycle
                p3[0] = lst[i]; p3[1] = u; p3[2] = lst[j];
                return p3;
            }
        }
        //Then bfs
        q.push(u);
        while(!q.empty()) {
            //cout << "3\n";
            w = q.front(); q.pop();
            //cout << "checking " << w << "\n";
            if (seen.find(w) != seen.end()) continue;
            seen.insert(w);
            for (int v : adj[w]) {
                for (int p : parents[w]) {
                    //cout << p << " is parent of " << w << "\n";
                    if (!find_i(adj[p], v) && p != v) {
                        p3[0] = p; p3[1] = w; p3[2] = v;
                        //sort(p3.begin(), p3.end());
                        return p3;
                    }
                }
                q.push(v);
                parents[v].push_back(w);
            }
        }
        //cout << "end for " << u << "\n";
    }
    //cout << "4\n";
    return {};
}

vector<tri> find_all_p3(vector<list<int>> &adj, info_struct &info) {
    set<tri> seen_p3s;
    vector<vector<int>> parents(info.n_);
    for (int i = 0; i < info.n_; i++) parents[i] = vector<int>();
    //cout << "1\n";
    set<int> seen;
    vector<int> p3(3);
    queue<int> q;
    int n_size;
    int w;
    int min_, max_;
    for (int u = 0; u < info.n_; u++) {
        if (seen.find(u) != seen.end()) continue;
        //Also look at neighbours u
        n_size = adj[u].size();
        //cout << "2\n";
        vector<int> lst;
        for (int i : adj[u]) lst.push_back(i);
        for (int i = 0; i < n_size - 1; i++) {
            for (int j = i+1; j < n_size; j++) {
                //cout << "n_, i, j: " << info.n_ << " " << i << " " << j << "\n";
                if (find_i(adj[lst[i]], lst[j])) continue; //we have a 3-cycle
                p3[0] = lst[i]; p3[1] = u; p3[2] = lst[j];
                min_ = min(p3[0], p3[2]);
                max_ = max(p3[2], p3[0]);
                seen_p3s.insert(tri(min_, p3[1], max_));
                //return p3;
            }
        }
        //Then bfs
        q.push(u);
        while(!q.empty()) {
            //cout << "3\n";
            w = q.front(); q.pop();
            //cout << "checking " << w << "\n";
            if (seen.find(w) != seen.end()) continue;
            seen.insert(w);
            for (int v : adj[w]) {
                for (int p : parents[w]) {
                    //cout << p << " is parent of " << w << "\n";
                    if (!find_i(adj[p], v) && p != v) {
                        p3[0] = p; p3[1] = w; p3[2] = v;
                        min_ = min(p3[0], p3[2]);
                        max_ = max(p3[2], p3[0]);
                        seen_p3s.insert(tri(min_, p3[1], max_));
                        //sort(p3.begin(), p3.end());
                        //return p3;
                    }
                }
                q.push(v);
                parents[v].push_back(w);
            }
        }
        //cout << "end for " << u << "\n";
    }
    vector<tri> to_return;
    for (tri t : seen_p3s) to_return.push_back(t);
    //cout << "4\n";
    return to_return;
}

void replace(list<int> &li, int a, int b) {
    auto it = li.begin();
    while(it != li.end()) {
        if (*it == a) *it = b;
        it++;
    }
}

bool check_no_duplicates(vector<list<int>> &adj) {
    set<int> s;
    for (int t = 0; t < adj.size(); t++) {
        s.clear();
        for (int i : adj[t]) {
            if (s.find(i) != s.end()) {
                cout << "Wrong for node " << t << "\n";
                for (int j : adj[t]) cout << j << " ";
                cout << "\n";
                return false;
            }
            s.insert(i);
        }
    }
    return true;
}

bool n_marked_in_adj(vector<list<int>> &adj, int n_) {
    for (int i = 0; i < adj.size(); i++) {
        for (int j : adj[i]) {
            if (j == n_) {
                cout << "n_ is in " << i << "\n";
                return true;
            }
        }
    }
    return false;
}


bool remove_edge(vector<list<int>> &adj, info_struct &info, int a, int b) {
    if (info.end_iteration) return false;
    info.progress.push_back(0);
    //if (true || info.k_ == 0) cout << "remove edge " << a << " " << b << "\n";
    //for (int i : adj[a]) cout << i << " "; cout << "\n";
    adj[a].remove(b); adj[b].remove(a);
    //for (int i : adj[a]) cout << i << " "; cout << "\n";
    info.k_++;
    bool result = recursion(adj, info);
    adj[a].push_back(b); adj[b].push_back(a);
    info.k_--;
    //if (!check_no_duplicates(adj)) {
    //    cout << "Duplicates found 4. n_, k_ = " << info.n_ << " " << info.k_ << "\n";
    //    exit(0);
    //}
    //cout << "add edge " << a << " " << b << "\n";
    info.progress.pop_back();
    return result;
}

bool add_edge(vector<list<int>> &adj, info_struct &info, int a, int b) {
    if (info.end_iteration) return false;
    info.progress.push_back(1);
    //if (true || info.k_ == 0) cout << "add edge " << a << " " << b << "\n";
    adj[a].push_back(b); adj[b].push_back(a);
    info.k_++;
    bool result = recursion(adj, info);
    adj[a].remove(b); adj[b].remove(a);
    info.k_--;
    //if (!check_no_duplicates(adj)) {
    //    cout << "Duplicates found 3. n_, k_ = " << info.n_ << " " << info.k_ << "\n";
    //    exit(0);
    //}
    //cout << "remove edge " << a << " " << b << "\n";
    info.progress.pop_back();
    return result;
}

bool split_vertex(vector<list<int>> &adj, info_struct &info, int a) {
    if (info.end_iteration) return false;
    info.progress.push_back(2);
    bool result = false;
    int sz = adj[a].size();
    list<int>::iterator it_i, it_j;
    for (int i = 1; i < pow(2, sz); i++) {
        for (int j = 1; j < pow(2, sz); j++) {
            //cout << i << " " << j << "\n";
            //cout << "i | j: " << (i | j) << "\n";
            //cout << "(1 << sz) - 1): " << (1 << sz) - 1 << "\n";
            if ((i | j) != (1 << sz) - 1) continue;
            
            adj.emplace_back(adj[a]);
            info.n_++;
            info.k_++;

            it_i = adj[a].begin();
            it_j = adj[info.n_-1].begin();
            int other;
            //cout << "size of adj: " << adj.size() << "\n";
            for (int t = 0; t < sz; t++) {
                if ((i & (1 << t)) == 0) {
                    other = *it_i;
                    it_i = adj[a].erase(it_i);
                    adj[other].remove(a);
                }
                else ++it_i;

                if ((j & (1 << t)) == 0) {
                    //other = *it_j;
                    it_j = adj[info.n_-1].erase(it_j);
                }
                else {
                    //cout << "*it_j: " << *it_j << "\n";
                    adj[*it_j].push_back(info.n_-1); 
                    ++it_j;
                }
            }
            //if (info.k_ == 1) {
                //cout << "node being split: " << a << "\n";
                //cout << "left edges: ";
                //for (int v : adj[a]) cout << v << " "; cout << "\n";
                //cout << "right edges: ";
                //for (int v : adj[info.n_-1]) cout << v << " "; cout << "\n";
            //}
            //if (!check_no_duplicates(adj)) {
            //    cout << "Duplicates found 1. n_, k_ = " << info.n_ << " " << info.k_ << "\n";
            //    exit(0);
            //}

            if (recursion(adj, info)) result = true;
            
            //Also add back edges from other vertices to a.
            for (int u : adj[info.n_-1]) {
                //cout << "replacing\n";
                //for (int k : adj[u]) cout << k << " "; cout << "\n";
                replace(adj[u], info.n_-1, a);
                //for (int k : adj[u]) cout << k << " "; cout << "\n";
                adj[u].sort();
                adj[u].unique();
            }
            it_i = adj[a].begin();
            adj[a].splice(it_i, adj[info.n_-1]);
            adj[a].sort();
            adj[a].unique();
            
            adj.pop_back();
            info.k_--;
            info.n_--;

            if (result) break;
        }
        //if (!check_no_duplicates(adj)) {
        //    cout << "Duplicates found 2. n_, k_ = " << info.n_ << " " << info.k_ << "\n";
        //    exit(0);
        //}

        //if (n_marked_in_adj(adj, info.n_)) {
        //   cout << "n_=" << info.n_ << " still in adj, a = " << a << "\n";
         //   exit(0);
        //}

        //if (adj[a].size() != sz) {
        //    cout << "Change in size for adjacency list of " << a << "\n";
        //    exit(0);
        //}
    }
    info.progress.pop_back();
    return result;
}

bool recursion(vector<list<int>> &adj, info_struct &info) {
    chrono::steady_clock::time_point current = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(current - info.last_checkpoint).count();
    if (time_elapsed / 1000000 > 10) {
        info.last_checkpoint = current;
        for (int i : info.progress) cout << i << " ";
        cout << endl;
    }

    optional<vector<int>> p3 = find_p3(adj, info);
    if (!p3.has_value()) return true;
    //cout << "found p3 " << p3.value()[0] << " " << p3.value()[1] << " " << p3.value()[2] << "\n";
    if (info.k_ == info.k) return false;

    time_elapsed = chrono::duration_cast<chrono::microseconds>(current - info.iteration_start).count();
    if (time_elapsed / 1000000 > 60) {
        info.end_iteration = true;
        return false;
    }

    bool result = remove_edge(adj, info, p3.value()[0], p3.value()[1]) || remove_edge(adj, info, p3.value()[1], p3.value()[2]) || \
        add_edge(adj, info, p3.value()[0], p3.value()[2]) || split_vertex(adj, info, p3.value()[1]);

    return result;
    //else, or between different ways to recurse
    //return result of the or.

}

int main () {
    string infile;
    cin >> infile;
    vector<vector<int>> adj_temp = read_gz_file(infile);
    cout << "Input read succesfully\n";
    int n = adj_temp.size();

    vector<list<int>> adj(n, list<int>());

    for (int i = 0; i < n; i++) {
        cout << i << ": "; 
        for (int j : adj_temp[i]) {
            adj[i].push_back(j);
            cout << j << " ";
        }
        cout << "\n";
    }

    cout << "Give k" << "\n";
    int k; cin >> k;

    info_struct info;
    info.n = n;
    info.n_ = n;
    info.k = k;
    info.k_ = 0;
    info.progress = vector<int>();
    info.last_checkpoint = chrono::steady_clock::now();
    info.end_iteration = false;

    bool result = false;

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();

    vector<tri> p3s = find_all_p3(adj, info);
    random_shuffle(p3s.begin(), p3s.end());

    //for (tri t : p3s) cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << "\n";
    for (int i = 0; i < p3s.size(); i++) {
        info.iteration_start = chrono::steady_clock::now();
        vector<int> p3 = {get<0>(p3s[i]), get<1>(p3s[i]), get<2>(p3s[i])};
        result = remove_edge(adj, info, p3[0], p3[1]) || remove_edge(adj, info, p3[1], p3[2]) || \
        add_edge(adj, info, p3[0], p3[2]) || split_vertex(adj, info, p3[1]);
        if (info.end_iteration) {
             cout << "Did not finish, answer still unknown\n";
             info.end_iteration = false;
        }
        else if (result) break;
        else {
            cout << "Algorithm terminated, NO is definite.\n";
            break;
        }
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();

    cout << "Time taken: " << time_elapsed / 1000000 << " seconds\n";

    if (result) cout << "YES\n";
    else cout << "NO\n";
}
