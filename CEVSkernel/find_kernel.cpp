#include "find_kernel.h"

//Note that cc means critical clique, not to be confused with
//connected components.

//Hashes a set of ints by summing its elements and taking modulo z.
int sum_hash(set<int> s, int z) {
    int sum_of_s = 0;
    for (int i : s) {
        sum_of_s += i;
    }
    return sum_of_s % z;
}

//Return true if the sets are equal, false if they are not.
bool compare_neighbourhoods(set<int> first, set<int> second) {
    if (first.size() != second.size()) return false;
    for (int u : first) {
        if (!(second.find(u) != second.end())) {
            return false;
        }
    }
    return true;
}

set<int> find_closed_neighbourhood(Graph &g, int &v) {
    set<int> to_return;
    to_return.insert(v);
    for (int w : g.adj[v]) {
        to_return.insert(w);
    }
    return to_return;
}

//Returns s hashed vector of vectors that contains sets of vertices in the
//same critical cliques. Z is the modulo with which to hash.
vector<vector<set<int>>> find_hashed_cc(Graph &g, int z) {
    int hash_mod = z;
    //In each set, stores vertices that have the same closed neighbourhood.
    vector<vector<set<int>>> hashed_cn(hash_mod);
    set<int> closed_neighbourhood;
    set<int> other_closed_neighbourhood;
    int hashed;
    bool new_cc;
    int index;
    set<int> new_set;
    for (int u = 0; u < g.n; u++) {
        new_cc = true;
        closed_neighbourhood = find_closed_neighbourhood(g, u);
        hashed = sum_hash(closed_neighbourhood, hash_mod);

        index = 0;
        for (set<int> cn : hashed_cn[hashed]) {
            //If the first element has the same neighbourhood, all elements in
            //the set has the same neighbourhood.
            int w = *cn.begin();
            other_closed_neighbourhood = find_closed_neighbourhood(g, w);
            if (compare_neighbourhoods(closed_neighbourhood, other_closed_neighbourhood)) {
                new_cc = false;
                break;
            }
            index += 1;
        }
        if (new_cc) {
            new_set = set<int>();
            new_set.insert(u);
            hashed_cn[hashed].push_back(new_set);
        }
        else {
            hashed_cn[hashed][index].insert(u);
        }
    }
    return hashed_cn;

}

vector<set<int>> find_connected_components(Graph &g) {
    vector<set<int>> components;
    set<int> component;
    vector<int> stack;
    vector<int> marked(g.n, 0);
    for (int v = 0; v < g.n; v++) {
        stack.clear();

        if (marked[v] == 1) {
            continue;
        }

        component = set<int>();
        stack.push_back(v);
        while(!stack.empty()) {

            int u = stack.back();
            stack.pop_back();

            if (marked[u] == 1){
                continue;
            }
            component.insert(u);
            marked[u] = 1;

            for (int w : g.adj[u]) {
                if (marked[w] == 0) {
                    stack.push_back(w);
                }
            }
        }
        components.push_back(component);
    }
    return components;
}

vector<vector<int>> alter_vector_set_int(vector<set<int>> &vec) {
    vector<vector<int>> to_return;
    vector<int> new_vec;
    for (set<int> s : vec) {
        new_vec = vector<int>();
        for (int i : s) {
            new_vec.push_back(i);
        }
        to_return.push_back(new_vec);
    }
    return to_return;
}

WeightedGraph create_kernel_graph(Graph &g, RevertKernel &revert, vector<set<int>> &remaining) {
    map<int, int> nodes_to_new_nodes;
    int num_nodes = remaining.size();
    int count = 0;
    vector<int> weights;
    for (set<int> s : remaining) {
        for (int v : s) {
            nodes_to_new_nodes[v] = count;
        }
        weights.push_back(s.size());
        count += 1;
    }

    vector<set<int>> adj_lst(remaining.size());
    for (int i = 0; i < g.n; i++) {
        for(int v : g.adj[i]) {
            int a = nodes_to_new_nodes[i];
            int b = nodes_to_new_nodes[v];
            if (a != b) {
                adj_lst[a].insert(b);
                adj_lst[b].insert(a);
            }
        }
    }

    vector<vector<int>> adj_lst_2 = alter_vector_set_int(adj_lst);

    return WeightedGraph(adj_lst_2, weights);

}

//Checks if a set of nodes of G is a cluster in G so that all nodes have
//edges to each other.
bool is_cluster(Graph &g, set<int> &nodes) {
    for (int i : nodes) {
        for (int j : nodes) {
            if (i <= j) continue;
            if (!g.has_edge(i, j)) {
                return false;
            }
        }
    } return true;
}


WeightedGraph find_kernel(Graph &g, int &k, RevertKernel &revert) {
    vector<vector<set<int>>> hashed_cc = find_hashed_cc(g, 409);
    vector<set<int>> connected_components = find_connected_components(g);
    int hashed;
    int to_remove;
    int fetch;
    int index;
    for (set<int> comp : connected_components) {
        if (!is_cluster(g, comp)) continue;
        hashed = sum_hash(comp, 409);
        index = 0;
        to_remove = -1;
        for (set<int> cc : hashed_cc[hashed]) {
            fetch = *cc.begin();
            set<int> closed_neighbourhood = find_closed_neighbourhood(g, fetch);
            if (compare_neighbourhoods(comp, closed_neighbourhood)) {
                to_remove = index;
                break;
            }
            index += 1;
        }
        if (to_remove > -1) {
            revert.isolated_cc.push_back(hashed_cc[hashed][index]);
            hashed_cc[hashed].erase(hashed_cc[hashed].begin() + index);
        }
    }

    vector<set<int>> remaining;
    for (vector<set<int>> vec : hashed_cc) {
        for (set<int> s : vec) {
            if (s.size() > 0) {
                remaining.push_back(s);
            }
        }
    }

    revert.other_cc = remaining;
    WeightedGraph wg = create_kernel_graph(g, revert, remaining);
    return wg;
}
    //For each remaining cc, give a node in the new graph. Index from nodes in G to
    //nodes in G'.
    //If there is an edge uv in E(G), add edge C(U) to C(V) in E(G').