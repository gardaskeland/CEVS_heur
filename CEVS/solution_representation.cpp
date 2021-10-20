
#include <map>
#include <set>
#include <vector>
#include <iostream>
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

void SolutionRepresentation::add(int node, int si) {
    set<int> s = clusters[si];
    s.insert(node);
    clusters[si] = s;
    s = node_in_clusters[node];
    s.insert(si);
    node_in_clusters[node] = s;
}

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
        cout << *it;
        s1.insert(*it);
        nodes_to_sets = node_in_clusters[*it];
        nodes_to_sets.erase(sj);
        nodes_to_sets.insert(si);
        node_in_clusters[*it] = nodes_to_sets;

    }
    clusters[si] = s1;
    clusters.erase(sj);
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


set<int> SolutionRepresentation::get_node_to_clusters(int vi) {
    return node_in_clusters[vi];
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
                    delete_counter += 1;
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
                     add_counter += 1;
                 }
             }
         } 
    }

    //vertex splitting: For each node, add the number of clusters it is in -1 to the counter.
    int sz;
    for (int i = 0; i < g.n; i++) {
        sz = node_in_clusters[i].size();
        if (sz > 0) {
            vs_counter += sz - 1;
        }
    }

    //For debug
    //printEdgeChanges(edge_deletions, edge_additions);

    tuple<int, int, int> result = tuple<int, int, int>(delete_counter, add_counter, vs_counter);
    return result;
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