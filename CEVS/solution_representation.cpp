#include "solution_representation.h"

using namespace std;

void SolutionRepresentation::initiate_shallow(ShallowSolution &sol) {
    clusters = sol.clusters;
    node_in_clusters = sol.node_in_clusters;
}

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

void SolutionRepresentation::initial_solution_complete_graph(int n) {
    number_nodes = n;
    set<int> nodes_in_set;
    set<int> set_of_sets_nodes_are_in;
    for (int i = 0; i < n; i++) {
        nodes_in_set.insert(i);
        set_of_sets_nodes_are_in = set<int>();
        set_of_sets_nodes_are_in.insert(0);
        node_in_clusters[i] = set_of_sets_nodes_are_in;
    }
    clusters[0] = nodes_in_set;
}

SolutionRepresentation SolutionRepresentation::deep_copy() {
    SolutionRepresentation new_sol;
    new_sol.number_nodes = number_nodes;
    new_sol.clusters = clusters;
    new_sol.node_in_clusters = node_in_clusters;
    new_sol.co_occurence = co_occurence;
    new_sol.book = book;
    new_sol.to_permute = to_permute;
    return new_sol;
}

/**
void SolutionRepresentation::calculate_node_in_clusters() {
    set<int> to_add;
    for (int u = 0; u < number_nodes; u++) {
        to_add = set<int>();
        for (auto it = clusters.begin(); it != clusters.end(); it++) {
            if ((it->second).find(u) != (it->second).end()) to_add.insert(it->first);
        }
        node_in_clusters[u] = to_add;
    }
}*/

int get_vertex_degree(Graph &g, int &v, set<int> &marked) {
    int counter = 0;
    for (int i : g.adj[v]) {
        if (!(marked.find(i) != marked.end())) {
            counter += 1;
        }
    }
    return counter;
}

void SolutionRepresentation::increase_co_occurence(int u, int v) {
    co_occurence[u][v] += 1;
    co_occurence[v][u] += 1;
}

void SolutionRepresentation::decrease_co_occurence(int u, int v) {
    co_occurence[u][v] -= 1;
    co_occurence[v][u] -= 1;
}

void SolutionRepresentation::initialise_co_occurence(int u, int v) {
    co_occurence[u][v] = 1;
    co_occurence[v][u] = 1;
}

void SolutionRepresentation::make_co_occurence(int n) {
    co_occurence.clear();
    for (auto it = clusters.begin(); it != clusters.end(); it++) {
        vector<int> set_nodes;
        for (int u : it->second) set_nodes.push_back(u);
        for (int i = 0; i < set_nodes.size() - 1; i++) {
            for (int j = i + 1; j < set_nodes.size(); j++) {
                map<int,int> &m = co_occurence[set_nodes[i]];
                if (m.find(set_nodes[j]) != m.end()) {
                    increase_co_occurence(set_nodes[i], set_nodes[j]);
                } else {
                    initialise_co_occurence(set_nodes[i], set_nodes[j]);
                }
            }
        }   
    }
}

void SolutionRepresentation::initial_solution_max_degree(Graph &g) {
    number_nodes = g.n;
    set<int> marked;
    while(marked.size() < g.n) {
        //cout << "size of marked: " << marked.size() << "\n";
        int max_degree = 0;
        int v_max = -1;
        for (int i = 0; i < g.n; i++) {
            if (marked.find(i) != marked.end()) continue;
            int degree = get_vertex_degree(g, i, marked);
            if (degree == 0) {
                set<int> singleton_set;
                singleton_set.insert(i);
                marked.insert(i);
                add_set(singleton_set);
            }
            if (degree > max_degree) {
                max_degree = degree;
                v_max = i;
            }
        }
        if (v_max == -1) continue;
        set<int> to_add;
        to_add.insert(v_max);
        marked.insert(v_max);
        for (int u : g.adj[v_max]) {
            to_add.insert(u);
            marked.insert(u);
        } 
        add_set(to_add);
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
    if (!(clusters.find(si) != clusters.end())) return;
    set<int> &set_i = clusters[si];
    if (set_i.find(node) != set_i.end()) return;
    for (int v : set_i) {
        if (v == node) continue;
        map<int,int> &m = co_occurence[node];
        if (m.find(v) != m.end()) {
            increase_co_occurence(node, v);
        } else {
            initialise_co_occurence(node, v);
        }
    }

    set<int> s = clusters[si];
    s.insert(node);
    clusters[si] = s;
    s = node_in_clusters[node];
    s.insert(si);
    node_in_clusters[node] = s;
    changed_set(si);
    book.modified_vertices.update_time(node, book.operation_number);
}

//slow. Change?
void SolutionRepresentation::remove(int node, int si) {
    for (int v : clusters[si]) {
        if (node == v) continue;
        decrease_co_occurence(node, v);
    }

    changed_set(si);
    book.modified_vertices.update_time(node, book.operation_number);
    set<int> s = clusters[si];
    if (s.size() > 1) {
        s.erase(node);
        clusters[si] = s;
    } else {
        clusters.erase(si);
    }
    s = node_in_clusters[node];
    s.erase(si);
    node_in_clusters[node] = s;
}

void SolutionRepresentation::merge(int si, int sj) {
    set<int> indices;
    for (int i : get_set_indices()) {
        indices.insert(i);
    }
    if (!(indices.find(si) != indices.end()) || !(indices.find(sj) != indices.end())) {
        return;
    }
    set<int> s1 = get_set(si);
    set<int> s2 = get_set(sj);
    
    //cout << "in sp: line 64";
    //print_solution();
    set<int> nodes_to_sets;
    for (set<int>::iterator it = s2.begin(); it != s2.end(); ++it) {
        //cout << *it;
        if (s1.find(*it) != s1.end()) continue;
        add(*it, si);
        nodes_to_sets = node_in_clusters[*it];
        nodes_to_sets.erase(sj);
        nodes_to_sets.insert(si);
        node_in_clusters[*it] = nodes_to_sets;
    }
    //clusters[si] = s1;
    //cout << "in sp: line 74: ";
    //print_solution();

    //Updating book, erasing sj.
    changed_set(sj);
    changed_set(si);
    remove_set(sj);

    //cout << "in sp: line 82";
    //print_solution();
    //?
    for (int i : get_set_indices()) {
        book.b_merge.map_merge_cost.erase(minmax(sj, i));
    }
    book.b_split.inner_cost.erase(sj);
}

void SolutionRepresentation::disjoint_split(int si, set<int> &set_1, set<int> &set_2) {
    //for (int u : set_1) {
    //    for (int v : set_2) {
    //        if (set_1.find(v) != set_1.end() || u >= v) continue;
    //        decrease_co_occurence(u, v);
    //    }
    //}

    changed_set(si);
    set<int> to_split = get_set(si);
    set<int> node_to_its_clusters;
    add_set(set_2);
    map<int,set<int>>::reverse_iterator it = clusters.rbegin();
    for (int u : set_2) {
        //print_solution();
        remove(u, si);
        //print_solution();
        node_to_its_clusters = node_in_clusters[u];
        node_to_its_clusters.erase(si);
        node_in_clusters[u] = node_to_its_clusters;
    }
    //clusters[si] = to_split;
}

int SolutionRepresentation::get_co_occurence(int u, int v) {
    map<int,int> &m = co_occurence[u];
    if (m.find(v) != m.end()) return m[v];
    return 0;
}

//TODO: test
bool SolutionRepresentation::simple_feasibility_check() {
    set<int> count_nodes;
    map<int, set<int>> clusters = get_clusters();
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        for (int i : it->second) count_nodes.insert(i);
    }
    //cout << "number nodes: " << number_nodes << "\n";
    //cout << "all nodes in count_nodes: ";
    //for (int i : count_nodes) cout << i << " ";
    //cout << "\n";
    if (count_nodes.size() == number_nodes) {
        return true;
    } else {
        return false;
    }
}

int SolutionRepresentation::num_sets() {
    return clusters.size();
}

int SolutionRepresentation::get_random_set_index() {
    int sz = clusters.size();
    int r = rand() % sz;
    auto it = clusters.begin();
    while(r > 0) {
        it++;
        r--;
    }
    return it->first;
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
    for (int u : s) {
        for (int v : s) {
            if (u >= v) continue;
            map<int,int> &m = co_occurence[u];
            if (m.find(v) != m.end()) {
                increase_co_occurence(u, v);
            } else {
                initialise_co_occurence(u, v);
            }
        }
    }

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
    vector<int> update_vec; for (int node : s) update_vec.push_back(node);
    book.modified_vertices.update_time(update_vec, book.operation_number);
}


void SolutionRepresentation::add_set_ind(int si, set<int> s) {
    for (int u : s) {
        node_in_clusters[u].insert(si);
        for (int v : s) {
            if (u >= v) continue;
            map<int,int> &m = co_occurence[u];
            if (m.find(v) != m.end()) {
                increase_co_occurence(u, v);
            } else {
                initialise_co_occurence(u, v);
            }
        }
    }

    clusters[si] = s;
    changed_set(si);
    vector<int> update_vec; for (int node : s) update_vec.push_back(node);
    book.modified_vertices.update_time(update_vec, book.operation_number);
}


void SolutionRepresentation::remove_set(int si) {
    set<int> indices;
    for (int i : get_set_indices()) {
        indices.insert(i);
    }
    if (!(indices.find(si) != indices.end())) {
        return;
    }
    for (int u : clusters[si]) {
        for (int v : clusters[si]) {
            if (u >= v) continue;
            decrease_co_occurence(u, v);
        }
    }
    set<int> s = clusters[si];
    for (int i : s) {
        set<int> &nic = node_in_clusters[i];
        nic.erase(si);
        node_in_clusters[i] = nic;
    }
    clusters.erase(si);
    vector<int> update_vec; for (int node : s) update_vec.push_back(node);
    book.modified_vertices.update_time(update_vec, book.operation_number);
}


vector<int> SolutionRepresentation::get_set_indices() {
    vector<int> vec;
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        vec.push_back(it->first);
    }
    return vec;
}

set<int> SolutionRepresentation::get_set_indices_as_set() {
    set<int> s;
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        s.insert(it->first);
    }
    return s;
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
tuple<int, int, int> SolutionRepresentation::cost_operations(Graph &g) {
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
                    //cout << "edge cost of " << i << " " << v << ": " << g.get_edge_cost(i, v) << "\n";
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
                     //cout << "edge cost of " << i << " " << *it << ": " << g.get_edge_cost(i, *it) << "\n";
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
            //cout << "g.get_node_weight for " << i << ": " << g.get_node_weight(i) << "\n";
        }
    }

    //For debug
    //printEdgeChanges(edge_deletions, edge_additions);

    tuple<int, int, int> result = tuple<int, int, int>(delete_counter, add_counter, vs_counter);
    return result;
}


int SolutionRepresentation::cost_solution(Graph &g) {
    tuple<int, int, int> result = cost_operations(g);
    return get<0>(result) + get<1>(result) + get<2>(result);
}


SolutionRepresentation SolutionRepresentation::copy_solution() {
    map<int, set<int>> new_clusters;
    new_clusters.insert(clusters.begin(), clusters.end());
    map<int, set<int>> new_nodes_in_clusters;
    new_nodes_in_clusters.insert(node_in_clusters.begin(), node_in_clusters.end());
    SolutionRepresentation new_sol(number_nodes, book.total_operations);
    new_sol.clusters = new_clusters;
    new_sol.node_in_clusters = new_nodes_in_clusters;
    return new_sol;
}

ShallowSolution SolutionRepresentation::get_shallow_solution() {
    return ShallowSolution(clusters, node_in_clusters);
}

bool SolutionRepresentation::verify_co_occurence() {
    vector<map<int, int>> in_solution;
    for (int i = 0; i < co_occurence.size(); i++) {
        in_solution.emplace_back(map<int, int>());
    }
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        for (int u : it->second) {
            for (int v : it->second) {
                if (u >= v) continue;
                map<int,int> &m = in_solution[u];
                if (m.find(v) != m.end()) {
                    m[v] += 1;
                } else {
                    m[v] = 1;
                }
            }
        }
    }
    for (int i = 0; i < co_occurence.size(); i++) {
        map<int, int> m_i = in_solution[i];
        map<int, int> m_j = co_occurence[i];
        for (map<int, int>::iterator it = m_i.begin(); it != m_i.end(); it++) {
            if (!(m_j.find(it->first) != m_j.end())) {
                cout << "ERROR: " << i << " and " << it->first << " are not counted in co_occurence";
            }
            if (m_j[it->first] != it->second) {
                cout << "ERROR: " << "the value of co-occurence between " << i << " and " << it->first << " is wrong: " \
                    "should be " << it->second << ", but is " << m_j[it->first] << "\n";
            }
        }
    }
    return true;
}


void SolutionRepresentation::print_solution() {
    cout << "[";
    int counter = 0;
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); ++it) {
        cout << "[" << counter++ << ": ";
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

void SolutionRepresentation::print_co_occurence() {
    for (int i = 0; i < co_occurence.size(); i++) {
        cout << i << ": ";
        map<int,int> &m = co_occurence[i];
        for (map<int,int>::iterator it = m.begin(); it != m.end(); it++) {
            cout << "[" << it->first << ": " << it->second << "], ";
        }
        cout << "]\n";
    }
}

int SolutionRepresentation::num_splits() {
    int count = 0;
    for (map<int, set<int>>::iterator it = node_in_clusters.begin(); it != node_in_clusters.end(); it++) {
        //for (int i : it-> second) cout << i << " ";
        //cout << "\n";
        count += (it->second).size() - 1;
    }
    return count;
}

struct cmp_hash {

    bool operator() (set<int> &left, set<int> &right) {
        //cout << "size left: " << left.size() << ", size right: " << right.size() << "\n";
        auto itl = left.begin(); auto itr = right.begin();
        while(itl != left.end() && itr != right.end()) {
            if (*itl < *itr) return true;
            if (*itl > *itr) return false;
            itl++; itr++;
        }
        return false;
    }
};

int SolutionRepresentation::solution_hash() {
    vector<set<int>> sets;
    for (auto it = clusters.begin(); it != clusters.end(); it++) {
        sets.push_back(it->second);
    }
    sort(sets.begin(), sets.end(), cmp_hash());

    int result = 0;
    int set_sum;
    int counter = 1;
    for (set<int> s : sets) {
        set_sum = 0;
        for (int i : s) set_sum += i;
        result = (result + (set_sum * counter++)) % (1 << 15);
    }
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