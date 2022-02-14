#include "functions.h"

//Not clear what they mean, use this
int cost_adding_edge(Graph &g, int u, int v) {
    int cost = 0;
    for (int w : g.adj[u]) {
        if (binary_search(g.adj[v], w).has_value()) continue;
        for (int x : g.adj[v]) {
            if (!g.has_edge(u, v)) cost += 1;
        }
    }
    return cost;
}

//
int cost_removing_edge(Graph &g, int u, int v) {
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

void make_permanent_or_forbidden(double pth, double fth, Graph &g, int u, int v) {
    //u and v
    int common_neighbourhood = 1;
    for (int i : g.adj[u]) {
        if (binary_search(g.adj[v], i).has_value()) common_neighbourhood += 1;
    }

    if (g.has_edge(u, v) && ((double)common_neighbourhood / (double)g.adj[u].size() >= pth || (double)common_neighbourhood / (double)g.adj[v].size() >= pth)) {
        g.set_forbid_permanent(u, v);
    }
    else if (!g.has_edge(u, v) && (double)common_neighbourhood / (double)g.adj[u].size() <= fth || (double)common_neighbourhood / (double)g.adj[v].size() <= fth) {
        g.set_forbid_permanent(u, v);
    }
}

optional<tuple<int, int, int>> bfs(Graph &g, int u) {
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
                if (!g.has_edge(v, parent[next] && next != parent[next])) {
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

optional<tuple<int, int, int>> find_conflict_triple(Graph &g) {
    optional<tuple<int, int, int>> result;
    for (int u = 0; u < g.n; u++) {
        result =  bfs(g, u);
       if (result.has_value()) break;
    }
    return result;
}

bool allows_splitting(Graph &g, tuple<int, int, int> split) {
    if (g.forbid_permanent[get<0>(split)][get<1>(split)] == 1 && g.forbid_permanent[get<0>(split)][get<2>(split)] == 1 \
        && g.forbid_permanent[get<1>(split)][get<2>(split)] == 1) {
            return true;
    }
    return false;
}

int greedy_heuristic(Graph &g) {
    optional<tuple<int, int, int>> conflict_triple;
    int cost_add;
    int cost_delete_1;
    int cost_delete_2;
    tuple<int, int, int> uvw;
    int u, v, w;
    //May not be totally accuate...
    int cost = 0;
    double pth = 2;
    double fth = 0.5;
    while (true) {
        conflict_triple = find_conflict_triple(g);
        if (!conflict_triple.has_value()) break;

        if (allows_splitting(g, conflict_triple.value())) {
            cost += 1;
            g.split_vertex(conflict_triple);
        } else {

            uvw = conflict_triple.value();
            u = get<0>(uvw); v = get<1>(uvw); w = get<2>(uvw);
            cost_add = cost_adding_edge(g, u, w);
            cost_delete_1 = cost_removing_edge(g, u, v);
            cost_delete_2 = cost_removing_edge(g, v, w);

            if (cost_add <= cost_delete_1 && cost_add <= cost_delete_2) {
                g.add_edge(u, w);
                cost += 1;
            }
            else if (cost_delete_1 < cost_delete_2) {
                g.remove_edge(u, v);
                cost += 1;
            } else {
                g.remove_edge(v, w);
                cost += 1;
            }
        }

        for (int i = 0; i < g.n - 1; i++) {
            if (g.adj[i].size() == 0) continue;
            for (int j = i+1; j < g.n; j++) {
                if (g.adj[j].size() == 0) continue;
                make_permanent_or_forbidden(pth, fth, g, i, j);
            }
        }

    }
    return cost;
}