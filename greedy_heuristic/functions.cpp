#include "functions.h"

//Not clear what they mean, use this
int cost_adding_edge(Graph_ &g, int u, int v) {
    int cost = 0;
    for (int w : g.adj[u]) {
        if (binary_search(g.adj[v], w).has_value()) continue;
        for (int x : g.adj[v]) {
            if (!g.has_edge(u, v)) cost += 1;
        }
    }
    /**
    int m = g.adj[u].size();
    for (int i = 0; i < m; i++) {
        for (int j = i + 1; j < m; j++) {
            if (!g.has_edge(g.adj[u][i], g.adj[u][j])) cost += 1;
        }
    }
    m = g.adj[v].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (binary_search(g.adj[u], g.adj[v][j]).has_value()) continue;
            if (!g.has_edge(g.adj[v][i], g.adj[v][j])) cost += 1;
        }
    }*/
    return cost;
}

//
int cost_removing_edge(Graph_ &g, int u, int v) {
    vector<int> common_neighbours;
    vector<int> other_elements;
    int cost = 0;
    for (int w : g.adj[u]) {
        if (binary_search(g.adj[v], w).has_value()) common_neighbours.push_back(w);
        else other_elements.push_back(w);
    }
    //sort(common_neighbours.begin(), common_neighbours.end());
    for (int w : g.adj[v]) {
        if (binary_search(common_neighbours, w).has_value()) continue;
        else other_elements.push_back(w);
    }
    for (int i : common_neighbours) {
        for (int j : other_elements) {
            if (g.has_edge(i, j)) cost += 1;
        }
    }
    return cost;
}

int make_permanent_or_forbidden(double pth, double fth, Graph_ &g, int u, int v) {
    //u and v
    int cost = 0;
    int common_neighbourhood = 2;
    //Either fix the vector or just use a vector of sets as the data structure for forbid-permanent instead.
    //Actually, this works quite nicely with the binary search vector methods we use.
    if (v >= g.forbid_permanent[u].size()) cout << "ERROR\n";
    if (g.forbid_permanent[u][v] == 1) return 0;
    for (int i : g.adj[u]) {
        if (binary_search(g.adj[v], i).has_value()) common_neighbourhood += 1;
    }

    double relative_to_u = (double)common_neighbourhood / (double)g.adj[u].size();
    double relative_to_v = (double)common_neighbourhood / (double)g.adj[v].size();
    //cout << "u: " << relative_to_u << ", v: " << relative_to_v << "\n";

    if (g.has_edge(u, v) && (relative_to_u >= pth ||  relative_to_v >= pth)) {
        //cout << "n: " << g.n << "\n";
        //cout << "forbid " << u << " " << v << "\n";
        if (!g.has_edge(u, v)) {
            g.add_edge(u, v);
            cost += 1;
        } 
        g.set_forbid_permanent(u, v);
    }
    else if (!g.has_edge(u, v) && (relative_to_u <= fth || relative_to_v <= fth)) {
        //cout << "n: " << g.n << "\n";
        //cout << "forbid " << u << " " << v << "\n";
        if (g.has_edge(u, v)) {
            g.remove_edge(u, v);
            cost += 1;
        }
        g.set_forbid_permanent(u, v);
    }
    return cost;
}

optional<tuple<int, int, int>> bfs(Graph_ &g, int u) {
    int next;
    deque<int> q;
    q.push_back(u);
    vector<int> parent;
    for (int i = 0; i < g.n; i++) {
        parent.push_back(i);
    }
    vector<bool> marked(g.n, false);
    marked[u] = true;
    while(!q.empty()) {
        next = q.front();
        q.pop_front();
        for (int v : g.adj[next]) {
            if (!marked[v]) {
                if ((!g.has_edge(v, parent[next] )&& next != parent[next])) {
                    return optional<tuple<int, int, int>>(tuple<int, int, int>(v, next, parent[next]));
                }
                q.push_back(v);
                parent[v] = next;
                marked[v] = true;
                
            }
        }
    }
    return optional<tuple<int, int, int>>();
}

optional<tuple<int, int, int>> find_conflict_triple(Graph_ &g) {
    optional<tuple<int, int, int>> result;
    /**
    vector<int> to_shuffle;
    for (int i = 0; i < g.n; i++) {
        to_shuffle.push_back(i);
    }
    auto rng = default_random_engine {};
    shuffle(to_shuffle.begin(), to_shuffle.end(), rng);
    */
    for (int u = 0; u < g.n; u++) {
        result =  bfs(g, u);
        if (result.has_value()) break;
    }
    return result;
}

bool allows_splitting(Graph_ &g, tuple<int, int, int> &split) {
    if (g.forbid_permanent[get<0>(split)][get<1>(split)] == 1 && g.forbid_permanent[get<0>(split)][get<2>(split)] == 1 \
        && g.forbid_permanent[get<1>(split)][get<2>(split)] == 1) {
            return true;
    }
    return false;
}

void bfs_cc(Graph_ &g, set<int> &marked, int u) {
    int next;
    deque<int> q;
    q.push_back(u);
    marked.insert(u);
    //cout << "\n";
    while(!q.empty()) {
        next = q.front();
        q.pop_front();
        for (int v : g.adj[next]) {
            if (marked.find(v) != marked.end()) continue;
            marked.insert(v);
            q.push_back(v);
        }
    }
}

int count_components(Graph_ &g) {
    int components = 0;
    set<int> marked;
    marked.insert(-1);
    for (int u = 0; u < g.n; u++) {
        if (binary_search(g.split_vertices, u).has_value()) continue;
        else if (marked.find(u) != marked.end()) continue;
        bfs_cc(g, marked, u);
        components += 1;
    }
    return components;
}

vector<set<int>> greedy_heuristic(Graph_ &g) {
    optional<tuple<int, int, int>> conflict_triple;
    int cost_add;
    int cost_delete_1;
    int cost_delete_2;
    int max_split = 100;
    tuple<int, int, int> uvw;
    int u, v, w;
    //May not be totally accurate...
    int cost = 0;
    double pth = 0.75;
    double fth = 0.25;
    int splits = 0;
    //not counting additions and deletions in make_permanent_or_forbidden
    int deletions = 0;
    int additions = 0;
    while (true) {
        conflict_triple = find_conflict_triple(g);
        if (!conflict_triple.has_value()) break;
        
        //g.print_graph();
        uvw = conflict_triple.value();
        u = get<0>(uvw); v = get<1>(uvw); w = get<2>(uvw);
        //cout << "p3 is " << u << ", " << v << ", " << w << "\n";
        cost_add = cost_adding_edge(g, u, w);
        cost_delete_1 = cost_removing_edge(g, u, v);
        cost_delete_2 = cost_removing_edge(g, v, w);
        
        //Make sure that we don't add an edge between vertices of the same origin
        if (g.find_origin(u) == g.find_origin(w)) {
            g.forbid_permanent[u][w] = 1;
        }

        if (allows_splitting(g, conflict_triple.value()) && splits < max_split) {
            //cout << "We split \n";
            cost += 1;
            splits += 1;
            g.split_vertex(conflict_triple);
        } else {

            if (g.forbid_permanent[u][w] < 1 && cost_add <= cost_delete_1 && cost_add <= cost_delete_2) {
                //cout << "We add\n";
                g.add_edge(u, w);
                //g.set_forbid_permanent(u, w);
                additions += 1;
                cost += 1;
            }
            else if (g.forbid_permanent[u][v] < 1 && cost_delete_1 < cost_delete_2) {
                //cout << "We delete 1\n";
                g.remove_edge(u, v);
                g.set_forbid_permanent(u, v);
                deletions += 1;
                cost += 1;
            } else {
                //cout << "We delete 2\n";
                g.remove_edge(v, w);
                g.set_forbid_permanent(v, w);
                deletions += 1;
                cost += 1;
            }
        }

        for (int i = 0; i < g.n - 1; i++) {
            if (g.adj[i].size() == 0) continue;
            for (int j = i+1; j < g.n; j++) {
                if (g.adj[j].size() == 0) continue;
                cost += make_permanent_or_forbidden(pth, fth, g, i, j);
            }
        }

    }
    cout << "Number of vertices created: " << g.n << "\n";
    cout << "Number of components: " << count_components(g) << "\n";
    cout << "additions, deletions, splits: " << additions << ", " << deletions << ", " << splits << "\n";
    /**
    cout << "parents: ";
    for (int p = 0; p < g.parents.size(); p++) {
        cout << "parent of " << p << " is " << g.parents[p] << "\n";
    }
    //exit(0);
    */
   cout << "Cost: " << cost << "\n";
    vector<set<int>> components = g.components();

    //g.print_graph();
    return components;
}