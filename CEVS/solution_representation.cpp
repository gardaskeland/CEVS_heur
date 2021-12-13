#include "solution_representation.h"

using namespace std;


void SolutionRepresentation::initial_solution(int n)
{
    number_nodes = n;
    set<int> nodes_in_set;
    set<int> set_of_sets_nodes_are_in;
    for (int i = 0; i < n; i++) {
        nodes_in_set = set<int>();
        nodes_in_set.insert(i);
        clusters[i] = nodes_in_set;
        set_of_sets_nodes_are_in = set<int>();
        set_of_sets_nodes_are_in.insert(i);
        node_in_clusters[i] = set_of_sets_nodes_are_in;
    }
}

set<int> SolutionRepresentation::get_set(int si) {
    return clusters[si];
}

void SolutionRepresentation::changed_set(int si) {
    set<int> neighbour_sets;
    for (int i : clusters[si]) {
        for (int j : node_in_clusters[i]) {
            neighbour_sets.insert(j);
        }
    }
    vector<int> vec;
    for (int i : neighbour_sets) {
        vec.push_back(i);
        //cout << "changed " << i << "\n";
    }
    book.modified_clusters.update_time(vec, book.operation_number);
}

//slow. Change?
void SolutionRepresentation::add(int node, int si) {
    set<int> s = clusters[si];
    s.insert(node);
    clusters[si] = s;
    s = node_in_clusters[node];
    s.insert(si);
    node_in_clusters[node] = s;
    changed_set(si);
}

//slow. Change?
void SolutionRepresentation::remove(int node, int si) {
    set<int> s = clusters[si];
    s.erase(node);
    clusters[si] = s;
    s = node_in_clusters[node];
    s.erase(si);
    node_in_clusters[node] = s;
}

void SolutionRepresentation::merge(int si, int sj) {
    set<int> s1 = get_set(si);
    set<int> s2 = get_set(sj);
    set<int> nodes_to_sets;
    for (set<int>::iterator it = s2.begin(); it != s2.end(); ++it) {
        //cout << *it;
        s1.insert(*it);
        nodes_to_sets = node_in_clusters[*it];
        nodes_to_sets.erase(sj);
        nodes_to_sets.insert(si);
        node_in_clusters[*it] = nodes_to_sets;
    }
    clusters[si] = s1;

    //Updating book, erasing sj.
    changed_set(sj);
    remove_set(sj);
    changed_set(si);
    for (int i : get_set_indices()) {
        book.b_merge.map_merge_cost.erase(minmax(sj, i));
    }
}

//TODO: test
bool SolutionRepresentation::simple_feasibility_check() {
    set<int> count_nodes;
    for (int i = 0; i < number_nodes; i++) {
        for (set<int>::iterator it = clusters[i].begin(); it != clusters[i].end(); ++it) {
            count_nodes.insert(*it);
        }
    }
    if (count_nodes.size() == number_nodes) {
        return true;
    } else {
        return false;
    }
}

int SolutionRepresentation::num_sets() {
    return clusters.size();
}


set<int> SolutionRepresentation::get_node_to_clusters(int vi) {
    return node_in_clusters[vi];
}

map<int, set<int>> SolutionRepresentation::get_clusters() {
    return clusters;
}

map<int, set<int>> SolutionRepresentation::get_node_in_clusters() {
    return node_in_clusters;
}

void SolutionRepresentation::add_set(set<int> s) {
    if (clusters.size() == 0) {
        clusters[0] = s;
        for (int i : s) {
            node_in_clusters[i].insert(0);
        }
        return;
    }
    map<int,set<int>>::reverse_iterator it = clusters.rbegin();
    int biggest = it->first;
    clusters[biggest + 1] = s;
    for (int i : s) {
        node_in_clusters[i].insert(biggest + 1);
    }
    changed_set(biggest + 1);
}


void SolutionRepresentation::add_set_ind(int si, set<int> s) {
    clusters[si] = s;
    changed_set(si);
}


void SolutionRepresentation::remove_set(int si) {
    if (!(clusters.find(si) != clusters.end())) {
        return;
    }
    set<int> s = clusters[si];
    for (int i : s) {
        set<int> nic = node_in_clusters[i];
        nic.erase(si);
        node_in_clusters[i] = nic;
    }
    clusters.erase(si);
}


vector<int> SolutionRepresentation::get_set_indices() {
    vector<int> vec;
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        vec.push_back(it->first);
    }
    return vec;
}


void printEdgeChanges(set<pair<int, int>> deletions, set<pair<int, int>> additions) {
    cout << "Edges deleted:\n";
    for (set<pair<int, int>>::iterator it = deletions.begin(); it != deletions.end(); ++it) {
        cout << it->first << " " << it->second << "\n";
    }
    cout << "Edges added:\n";
    for (set<pair<int, int>>::iterator it = additions.begin(); it != additions.end(); ++it) {
        cout << it->first << " " << it->second << "\n";
    }
}


void print_integer_set(set<int> s) {
    cout << "[";
    for (set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        cout << *it << ", ";
    }
    cout << "]";
}

//TODO: Change so that it uses the weights of edges and vertices.
tuple<int, int, int> SolutionRepresentation::cost_operations(Graph g) {
    set<pair<int, int>> edge_deletions;
    set<pair<int, int>> edge_additions;
    int delete_counter = 0;
    int add_counter = 0;
    int vs_counter = 0;
    map<int, set<int>> node_in_cluster_with;

    //Iteate over all nodes and store which nodes they are supposed to be in a cluster with.
    set<int> temp;
    for (int i = 0; i < g.n; i++) {
        temp = set<int>();
        for (set<int>::iterator it = node_in_clusters[i].begin(); it != node_in_clusters[i].end(); ++it) {
            temp.insert(clusters[*it].begin(), clusters[*it].end());
        }
        node_in_cluster_with[i] = temp;
    }
    /**
    for (int i = 0; i < g.n; i++) {
        cout << "node " << i << " in clusters with ";
        print_integer_set(node_in_cluster_with[i]);
        cout << "\n";
    }
    */

    //edge deletions: For each node v in N(u), is v in the same cluster?
    pair<int, int> edge;
    pair<int, int> opposite;
    for (int i = 0; i < g.n; i++) {
        for (int v : g.adj[i]) {
            if (node_in_cluster_with[i].find(v) != node_in_cluster_with[i].end()) {
                continue;
            } else {
                edge = pair<int, int>(i, v);
                if (edge_deletions.find(edge) != edge_deletions.end()) {
                    continue;
                } else {
                    opposite = pair<int, int>(v, i);
                    edge_deletions.insert(edge);
                    edge_deletions.insert(opposite);
                    delete_counter += g.get_edge_cost(i, v);
                }
            }
        }
    }

    //edge additions: For each node v in the same cluster as u, does it have and edge?
    for (int i = 0; i < g.n; i++) {
        for (set<int>::iterator it = node_in_cluster_with[i].begin();
         it != node_in_cluster_with[i].end(); ++it) {
             if (i == *it) {
                 continue;
             }
             if (g.has_edge(i, *it)) {
                 continue;
             } else {
                 edge = pair<int, int>(i, *it);
                 if (edge_additions.find(edge) != edge_additions.end()) {
                     continue;
                 } else {
                     opposite = pair<int, int>(*it, i);
                     edge_additions.insert(edge);
                     edge_additions.insert(opposite);
                     add_counter += g.get_edge_cost(i, *it);
                 }
             }
         } 
    }

    //vertex splitting: For each node, add the number of clusters it is in -1 to the counter.
    int sz;
    for (int i = 0; i < g.n; i++) {
        sz = node_in_clusters[i].size();
        if (sz > 0) {
            vs_counter += g.get_node_weight(i) * (sz - 1);
        }
    }

    //For debug
    //printEdgeChanges(edge_deletions, edge_additions);

    tuple<int, int, int> result = tuple<int, int, int>(delete_counter, add_counter, vs_counter);
    return result;
}


int SolutionRepresentation::cost_solution(Graph g) {
    tuple<int, int, int> result = cost_operations(g);
    return get<0>(result) + get<1>(result) + get<2>(result);
}


SolutionRepresentation SolutionRepresentation::copy_solution() {
    map<int, set<int>> new_clusters;
    new_clusters.insert(clusters.begin(), clusters.end());
    map<int, set<int>> new_nodes_in_clusters;
    new_nodes_in_clusters.insert(node_in_clusters.begin(), node_in_clusters.end());
    SolutionRepresentation new_sol(number_nodes);
    new_sol.clusters = new_clusters;
    new_sol.node_in_clusters = new_nodes_in_clusters;
    return new_sol;
}


void SolutionRepresentation::print_solution() {
    cout << "[";
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); ++it) {
        cout << "[" << it->first << ": ";
        for (int i : it->second) {
            cout << i << ", ";
        }
        cout << "], ";
    }
    cout << "]\n";
}

void SolutionRepresentation::print_node_in_clusters() {
    cout << "[";
    for (map<int, set<int>>::iterator it = node_in_clusters.begin(); it != node_in_clusters.end(); ++it) {
        cout << "[" << it->first << ": ";
        if ((it->second).size() > 0) {
            for (int i : it->second) {
                cout << i << ", ";
            }
        }
        cout << "], ";
    }
    cout << "]\n";
}

/**
int main() {
    cout << "RUNNING...";
    SolutionRepresentation sr;
    sr.initial_solution(10);

    set<int>::iterator it;
    //it = sr.get_set(3).find(3);
    //if (it == sr.get_set(3).end()) {
    //    cout << "could not find the element";
    //}
    //else {
     //   sr.get_set(3).erase(it);
      //  cout << "ok";
    //}
    /**
    sr.remove(3, 3);
    //sr.remove(3, 4);
    //sr.remove(2, 3);
    //sr.remove(0, 0);
    for (int i = 0; i < 10; i++) {
        set<int> s = sr.get_set(i);
        for (set<int>::iterator it = s.begin(); it != s.end(); ++it) {
            std::cout << ' ' << *it;
        }
        std::cout << "\n";
        s = sr.node_in_clusters[i];
        for (set<int>::iterator it = s.begin(); it != s.end(); ++it) {
            std::cout << ' ' << *it;
        }
        std::cout << "\n";
    }
    sr.add(3, 5);
    sr.add(4, 5);
    set<int> s = sr.get_set(5);
    for (set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << "\n";
    s = sr.node_in_clusters[4];
    for (set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << "\n";

    sr.merge(5, 2);
    cout << "\n";
    set<int> s = sr.get_set(2);
    for (set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << "\n";
    s = sr.node_in_clusters[2];
    for (set<int>::iterator it = s.begin(); it != s.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << "\n";
}
*/