#include "find_kernel.h"

//Note that cc means critical clique, not to be confused with
//connected components.

//Hashes a set of ints by summing its elements and taking modulo z.
int sum_hash(set<int> s, int z) {
    int sum_of_s = 0;
    for (int i : s) {
        sum_of_s  = (sum_of_s + i) % z;
    }
    return sum_of_s;
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

//Inefficient. Better to copy the entire adjacency graph once and
//add v to N(v) for each vertex v.
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

//Changes a vector of sets into a vector of vectors.
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

/**
 * Creates the critical clique graph where each clique in remaining corresponds to one vertex
 * in the returned graph. Two vertices have an edge between them if there is an edge between
 * all vertices in the union of the corresponding cliques. Each vertex has the size of
 * its corresponding critical clique as weight.
 */
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
    //cout << "ok5";

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
    //cout << "ok6";

    vector<vector<int>> adj_lst_2 = alter_vector_set_int(adj_lst);
    
    //Should remove this for optimisation. 
    int num_edges = 0;
    for (vector<int> vec : adj_lst_2) {
        for (int i: vec) {
            num_edges += 1;
        }
    }
    //cout << "ok7";
    //Since each edge is counted twice.
    revert.num_edges = num_edges / 2;

    //cout << "ok8";

    return WeightedGraph(adj_lst_2, weights);

}

//Checks if a set of nodes of G is a cluster in G so that all nodes have
//edges to each other.
bool is_cluster(Graph &g, set<int> &nodes) {
    for (int i : nodes) {
        for (int j : nodes) {
            if (i <= j) continue;
            //cout << j;
            if (!g.has_edge(i, j)) {
                return false;
            }
        }
    } return true;
}

/**
 * Reduction explained: There is an optimal solution with pendant vertices in a cost by itself unless
 * the following occurs for the pendant vertices: Its neighbour also has degree 1 (component is K_2), or
 * its neighbour has only one other neighbour that also is a pendant vertex (best to add edge between
 * pendant vertices). This reduction may cause the cost found by using the heuristic on a critical clique
 * graph to not be accurate (may want to add this to the cost in alns2).
 */
Graph pendant_vertex_reduction(Graph &g, RevertKernel &revert) {
    vector<bool> remove_vertex(g.n, false);
    //can add sets we remove to isolated_cc.
    int count_neighbours, u, count;
    int found = true; //true if we keep finding pendant vertices
    int cost = 0;
    int vertices_removed = 0;
    while(found) {
        found = false;
        for (u = 0; u < g.n; u++) {
            if (remove_vertex[u]) continue;
            count_neighbours = 0;
            for (int v : g.adj[u]) {
                if (!remove_vertex[v]) count_neighbours += 1;
            }
            if (count_neighbours == 0) {
                remove_vertex[u] = true; 
                vertices_removed++;
                set<int> to_insert = {u};
                revert.removed_sets.push_back(to_insert);
                found = true;
            }
            if (count_neighbours == 1) {
                found = true;
                //not counted as neighbour of v 
                remove_vertex[u] = true; 
                count = 0;
                while(remove_vertex[g.adj[u][count]]) count++;
                int v = g.adj[u][count];
                count_neighbours = 0;
                int w;
                for (int x : g.adj[v]) {
                    if (remove_vertex[x]) continue;
                    count_neighbours++;
                    w = x;
                }
                
                //We remove K_2 of u and v from the graph
                if (count_neighbours == 0) {
                    set<int> to_insert = {u, v};
                    remove_vertex[v] = true;
                    revert.removed_sets.push_back(to_insert);
                    vertices_removed += 2;
                }
                //We remove isolated p3 u, v, w from graph
                else if (count_neighbours == 1) {
                    set<int> to_insert = {u, v, w};
                    remove_vertex[v] = true; remove_vertex[w] = true;
                    revert.removed_sets.push_back(to_insert);
                    vertices_removed += 3; cost += 1; // add edge between u and w.
                }
                //we simply remove u.
                else {
                    set<int> to_insert = {u};
                    revert.removed_sets.push_back(to_insert);
                    vertices_removed += 1; cost += 1; // remove edge of u
                }
            }
        }
    }
    //return modified graph
    map<int, int> relabelling;
    map<int, int> reverse_relabelling;
    count = 0;
    for(int u = 0; u < g.n; u++) {
        if (remove_vertex[u]) continue;
        relabelling[u] = count;
        reverse_relabelling[count++] = u;
    }

    revert.reverse_pedant_reduction_indices = reverse_relabelling;

    vector<vector<int>> new_adj_list;
    vector<int> to_relabel;
    for (int u = 0; u < g.n; u++) {
        if (remove_vertex[u]) continue;
        to_relabel = g.adj[u];
        for (int i = 0; i < to_relabel.size(); i++) {
            to_relabel[i] = relabelling[to_relabel[i]];
        }
        new_adj_list.push_back(to_relabel);
    }
    Graph g_(new_adj_list);
    return g_;
}


WeightedGraph find_critical_clique_graph(Graph &g, RevertKernel &revert) {
    //To do the reduction: Make a new graph 'g, store removed sets/vertices in isolated_cc.
    //probably a lot slower than is possible

    Graph g_ = pendant_vertex_reduction(g, revert);
    vector<vector<set<int>>> hashed_cc = find_hashed_cc(g_, 1000000);
    //cout << "ok";
    vector<set<int>> connected_components = find_connected_components(g_);
    //cout << "ok";
    int hashed;
    int to_remove;
    int fetch;
    int index;
    for (set<int> comp : connected_components) {
        if (!is_cluster(g_, comp)) continue;
        //cout << "ok1";
        hashed = sum_hash(comp, 1000000);
        index = 0;
        to_remove = -1;
        for (set<int> cc : hashed_cc[hashed]) {
            fetch = *cc.begin();
            set<int> closed_neighbourhood = find_closed_neighbourhood(g_, fetch);
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
    //Can add a function here to remove vertices of degree 1 / isolated P3's and put them in isolated_cc. NO! Need to do it in create_kernel_graph
    //cout << "ok2";

    vector<set<int>> remaining;
    for (vector<set<int>> vec : hashed_cc) {
        for (set<int> s : vec) {
            if (s.size() > 0) {
                remaining.push_back(s);
            }
        }
    }
    //cout << "ok3";

    revert.other_cc = remaining;
    WeightedGraph wg = create_kernel_graph(g_, revert, remaining);
    //cout << "ok4";
    return wg;
}
    //For each remaining cc, give a node in the new graph. Index from nodes in G to
    //nodes in G'.
    //If there is an edge uv in E(G), add edge C(U) to C(V) in E(G').


map<int, set<int>> relabel_map_of_sets(map<int, set<int>> &ma, RevertKernel &revert) {
    map<int, set<int>> new_ma;
    map<int, int> &relabelling = revert.reverse_pedant_reduction_indices;
    set<int> new_set;
    for (auto it = ma.begin(); it != ma.end(); it++) {
        for (int i : it-> second) new_set.insert(relabelling[i]);
        new_ma[relabelling[it->first]] = new_set;
        new_set.clear();
    }
    return new_ma;
}

ShallowSolution from_cc_sol_to_sol(Graph &g, ShallowSolution &sol, RevertKernel &revert) {
    ShallowSolution to_return;
    set<int> next_cluster;
    vector<set<int>> node_to_cluster = vector<set<int>>(g.n, set<int>());
    int counter = 0;
    //cout << "ok0\n";
    for (map<int, set<int>>::iterator it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        next_cluster = set<int>();
        for (int i : it->second) {
            for (int j : revert.other_cc[i]) {
                next_cluster.insert(j);
                node_to_cluster[j].insert(counter);
            }
        }
        to_return.clusters[counter] = next_cluster;
        counter += 1;
    }
    //cout << "ok1\n";
    for (set<int> s : revert.isolated_cc) {
        to_return.clusters[counter] = s;
        for (int i : s) {
            node_to_cluster[i].insert(counter);
        }
        counter += 1;
    }
    //cout << "ok2\n";
    for (int i = 0; i < node_to_cluster.size(); i++) {
        to_return.node_in_clusters[i] = node_to_cluster[i];
    }
    
    //Relabels and inserts removed sets to revert the pendant vertex reduction
    to_return.clusters = relabel_map_of_sets(to_return.clusters, revert);
    to_return.node_in_clusters = relabel_map_of_sets(to_return.node_in_clusters, revert);

    for (set<int> s : revert.removed_from_cc) {

    }

    return to_return;
}