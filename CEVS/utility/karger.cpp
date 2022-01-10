#include "karger.h"


int find_(map<int, int> &uf, int &u) {
    int current = u;
    vector<int> seen;
    while(uf[current] != current) {
        seen.push_back(current);
        current = uf[current];
    }
    //path compression
    for (int i : seen) {
        uf[i] = current;
    }
    return current;
}

void union_(map<int, int> &uf, int &u, int &v) {
    int s1 = find_(uf, u);
    int s2 = find_(uf, v);
    uf[s1] = s2;
}

class Edge {
    public:
        int u;
        int v;

    Edge(int &a, int &b) {
        u = a;
        v = b;
    }
};

//TODO
vector<Edge> find_edges(Graph&g, set<int> vertices) {
    set<pair<int, int>> seen_edges;
    vector<Edge> edges;
    for (int u : vertices) {
        for (int v : g.adj[u]) {
            if (vertices.find(v) != vertices.end() && !(seen_edges.find(pair<int, int>(u, v)) != seen_edges.end())) {
                seen_edges.insert(pair<int, int>(u, v));
                seen_edges.insert(pair<int, int>(v, u));
                edges.push_back(Edge(u, v));
            }
        }
    } return edges;
}

// Inspired by https://www.geeksforgeeks.org/kargers-algorithm-for-minimum-cut-set-1-introduction-and-implementation/ 16.12.2021.
pair<int, pair<set<int>, set<int>>> find_min_cut(Graph &g, SolutionRepresentation &sol, int &si) {
    set<int> vertices = sol.get_set(si);
    vector<Edge> edges = find_edges(g, vertices);
    map<int, int> uf;
    int num_components;
    pair<set<int>, set<int>> min_cut;
    int min_cut_value = pow(2, 31);
    int edge_vector_size;
    int miss_counter;
    vector<Edge> remaining_edges;
    double max_ = vertices.size();
    for (double x = 0; x < max_; x++) {
        remaining_edges = edges;
        uf.clear();
        for (int u : vertices) {
            uf[u] = u;
        }
        edge_vector_size = edges.size();
        num_components = vertices.size();

        while(num_components > 2) {
            //Removes self-loops if lots of misses happen.
            if (miss_counter >= edge_vector_size) {
                vector<Edge> new_edges;
                for (Edge e : remaining_edges) {
                    if (find_(uf, e.u) == find_(uf, e.v)) {
                        continue;
                    }
                    new_edges.push_back(e);
                }
                edge_vector_size = new_edges.size();
                remaining_edges = new_edges;
                miss_counter = 0;
            }

            int r = rand() % edge_vector_size;

            Edge cur = remaining_edges[r];
            
            if (find_(uf, cur.u) == find_(uf, cur.v)) {
                miss_counter += 1;
                continue;
            }
            union_(uf, cur.u, cur.v);
            num_components -= 1;
            
        }
        int size_min_cut = 0;
        for (Edge e : remaining_edges) {
            //use mapping here
            if (find_(uf, e.u) != find_(uf, e.v)) size_min_cut += g.get_edge_cost(e.u, e.v); 
        }
        if (size_min_cut < min_cut_value) {
            min_cut_value = size_min_cut;
            int set_id_1 = uf.begin()->second;
            set<int> set_1;
            set<int> set_2;
            for (map<int, int>::iterator it = uf.begin(); it != uf.end(); it++) {
                if (it->second == set_id_1) {
                    set_1.insert(it->first);
                } else {
                    set_2.insert(it->first);
                }
            }
            min_cut = pair<set<int>, set<int>>(set_1, set_2);
        }
    }
    return pair<int, pair<set<int>, set<int>>>(min_cut_value, min_cut); 
}