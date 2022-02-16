#include "graph_.h"
#include <optional>

bool Graph_::has_edge(int u, int v) {
    return binary_search(adj[u], v).has_value();
}

void Graph_::add_edge(int u, int v) {
    binary_insert(adj[u], v);
    binary_insert(adj[v], u);
}

void Graph_::remove_edge(int u, int v) {
    vector<int> &adjref = adj[u];
    optional<int> pos = binary_search(adjref, v);
    //cout << "pos has value: " << pos.has_value() << "\n";
    //if (pos.has_value()) cout << "the value is " << pos.value() << "\n";
    if (pos.has_value()) {
        adjref.erase(adjref.begin() + pos.value());
        vector<int> &adj_v = adj[v];
        pos = binary_search(adj_v, u);
        //cout << "pos has value: " << pos.has_value() << "\n";
        //if (pos.has_value()) cout << "the value is " << pos.value() << "\n";
        adj_v.erase(adj_v.begin() + pos.value());
    }
}

void Graph_::set_forbid_permanent(int u, int v) {
    forbid_permanent[u][v] = 1;
    forbid_permanent[v][u] = 1;
}

void Graph_::split_vertex_(int u, int v, int w) {
    optional<int> j;
    int counter_u = 0; int counter_w = 0;
    vector<int> uv_set;
    vector<int> wv_set;
    for (int i : adj[u]) {
        j = binary_search(adj[v], i);
        if (j.has_value()) {
            counter_u += 1;
            uv_set.push_back(i);
        }
    }
    for (int i : adj[w]) {
        j = binary_search(adj[v], i);
        if (j.has_value()) {
            counter_w += 1;
            wv_set.push_back(i);
        }
    }

    int best, worst;
    if (counter_u >= counter_w) {
        best = u;
        worst = w;
    }
    else {
        best = w;
        worst = u;
    }

    //Add v1 and v2
    vector<int> n_v = adj[v];
    vector<int> v1_set;
    if (best == u) v1_set = uv_set;
    else v1_set = wv_set;
    //N[u]
    v1_set.push_back(best);
    sort(v1_set.begin(), v1_set.end());
    adj.push_back(v1_set);
    forbid_permanent.emplace_back(vector<int>(n+1, 0));
    parents.emplace_back(v);
    vertices.emplace_back(n);
    int v1 = n;
    n = n+1;

    vector<int> v2_set = vector<int>();
    for (int i : adj[v]) {
        if (i == u) continue;
        if (!binary_search(v1_set, i).has_value()) v2_set.push_back(i);
    }
    sort(v2_set.begin(), v2_set.end());
    adj.push_back(v2_set);
    forbid_permanent.emplace_back(vector<int>(n+1, 0));
    parents.emplace_back(v);
    vertices.emplace_back(n);
    int v2 = n;
    n = n+1;

    for (vector<int> &vec : forbid_permanent) {
        while (vec.size() < n) {
            vec.push_back(0);
        }
    }
    /**
    //Delete edges from N(v1) to N(v2)
    optional<int> pos;
    for (int i : v1_set) {
        for (int x : v2_set) {
            if (has_edge(i, x)) remove_edge()
        }
    }*/

    //move all edges to v to v1 and v2
    //print_graph();
    for (int i = 0; i < n - 2; i++) {
        if (i == v) continue;
        j = binary_search(adj[i], v);
        if (j.has_value()) {
            adj[i].erase(adj[i].begin() + j.value());
            //if (i == 0) cout << "remove 4 here\n";
            //print_graph();
            j = binary_search(v1_set, i);
            if (j.has_value()) {
                //if (i == 0) cout << "Adding " << v1 << " to " << i << "\n";                         
                binary_insert(adj[i], v1);
            }
            j = binary_search(v2_set, i);
            if (j.has_value()) {
                binary_insert(adj[i], v2);
            }
        }
    }

    //Delete references from v to other vertices.
    adj[v].clear();
    binary_insert(split_vertices, v);
}

void Graph_::split_vertex(optional<tuple<int, int, int>> t) {
    tuple<int, int, int> v = t.value();
    split_vertex_(get<0>(v), get<1>(v), get<2>(v));
}

int Graph_::find_origin(int u) {
    int temp = u;
    while (temp != parents[temp]) {
        //cout << temp << "\n";
        temp = parents[temp];
    }
    //cout << "end\n";
    return temp;
}

void Graph_::print_graph() {
    cout << "Printing graph: \n";
    for (int i = 0; i < n; i++) {
        cout << i << ": ";
        for (int j : adj[i]) {
            cout << j << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

set<int> Graph_::bfs(set<int> &marked, int u) {
    set<int> s;
    int org;
    int v;
    deque<int> q;
    q.push_back(u);
    org = find_origin(u);
    marked.insert(org);
    s.insert(org);
    while(!q.empty()) {
        v = q.front();
        q.pop_front();
        for (int w : adj[v]) {
            org = find_origin(w);
            if (marked.find(org) != marked.end()) continue;
            q.push_back(w);
            marked.insert(org);
            s.insert(org);
        }
    }
    return s;
}

vector<set<int>> Graph_::components() {
    vector<set<int>> result;
    set<int> marked;
    for (int u = 0; u < n; u++) {
        if (marked.find(find_origin(u)) != marked.end()) continue;
        if (binary_search(split_vertices, u).has_value()) continue;
        result.emplace_back(bfs(marked, u));
    }
    return result;
}

