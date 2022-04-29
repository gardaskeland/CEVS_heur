#include "graph.h"

#define tri tuple<int, int, int>

bool Graph::has_edge(int u, int v) {
    if (adj_matrix.size() > 0 && adj_matrix[u][v] == 1) {
        return true;
    }
    else if (adj_matrix.size() == 0) {
        for (int w : adj[u]) {
            if (w == v) return true;
            }
        {
            /* code */
        }
    }
    return false;
}

int Graph::get_edge_cost(int u, int v) {
    return 1;
}

int Graph::get_node_weight(int u) {
    return 1;
}

int Graph::get_number_nodes() {
    return n;
}

void Graph::find_all_p3s() {
    set<tri> seen_p3s;
    vector<vector<int>> parents(n, vector<int>());
    set<int> seen;
    vector<int> p3(3);
    queue<int> q;
    int n_size;
    for (int u = 0; u < n; u++) {
        if (seen.find(u) != seen.end()) continue;
        //Also look at neighbours u
        n_size = adj[u].size();
        for (int i = 0; i < n_size - 1; i++) {
            for (int j = i+1; j < n_size; j++) {
                if (has_edge(adj[u][i], adj[u][j])) continue; //we have a 3-cycle
                p3[0] = adj[u][i]; p3[1] = u; p3[2] = adj[u][j];
                sort(p3.begin(), p3.end());
                seen_p3s.insert(tri(p3[0], p3[1], p3[2]));
            }
        }
        //Then bfs
        q.push(u);
        while(!q.empty()) {
            u = q.front(); q.pop();
            //cout << "checking u\n";
            if (seen.find(u) != seen.end()) continue;
            seen.insert(u);
            for (int v : adj[u]) {
                for (int p : parents[u]) {
                    if (!has_edge(p, v) && p != v) {
                        p3[0] = p; p3[1] = u; p3[2] = v;
                        sort(p3.begin(), p3.end());
                        seen_p3s.insert(tri(p3[0], p3[1], p3[2]));
                    }
                }
                q.push(v);
                parents[v].push_back(u);
            }
        }
    }

    for (tri t : seen_p3s) {
        all_p3s.push_back(t);
    }
}