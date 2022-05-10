#include "find_solution_hash.h"

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

int calculate_hash_add_node_to_neighbours(SolutionRepresentation &sol) {
    int result = 0;
    int set_sum;
    int node_to_add = sol.book.b_add_node.v;
    vector<int> &sets = sol.book.b_add_node.sets_to_change;
    sort(sets.begin(), sets.end());
    int index = 0;
    int counter = 1;
    vector<set<int>> vec;
    set<int> s;
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        set<int> s = it->second;
        if (index < sets.size() && it->first == sets[index]) {
            s.insert(node_to_add);
            index++;
        }
        vec.push_back(s);
    }
    sort(vec.begin(), vec.end(), cmp_hash());
    for (set<int> s_it : vec) {
        set_sum = 0;
        for (int i : s_it) set_sum += i;
        result = (result + (set_sum * counter++)) % (1 << 15);
    }
    return result;
}

int calculate_hash_add_node_to_set(SolutionRepresentation &sol) {
    int result = 0;
    int set_sum;
    int counter = 1;
    
    vector<set<int>> vec;
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        if (it->first == sol.book.b_add_node.si) {
            set<int> s = it->second;
            s.insert(sol.book.b_add_node.v);
            vec.push_back(s);
        }
        else vec.push_back(it->second);
    }

    sort(vec.begin(), vec.end(), cmp_hash());

    for (set<int> s_it : vec) {
        set_sum = 0;
        for (int i : s_it) set_sum += i;
        result = (result + (set_sum * counter++)) % (1 << 15);
    }

    return result;
}

int calculate_hash_remove_node(SolutionRepresentation &sol) {
    int result = 0;
    int set_sum;
    int node_to_remove = sol.book.b_add_node.v;
    vector<int> &sets = sol.book.b_add_node.sets_to_change;
    sort(sets.begin(), sets.end());
    int index = 0;
    int counter = 1;
    vector<set<int>> vec;
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        if (it->first == sets[index]) {
            set<int> s = it->second;
            //cout << "node_to_remove: " << node_to_remove << "\n";
            //cout << "set content " << "\n";
            //for (int i : s) cout << i << " ";
            //cout << "\n";
            //A bit haphazard, should not be needed
            auto it_ = s.find(node_to_remove);
            if (it_ != s.end()) {
                s.erase(it_);
            }
            vec.push_back(s);
            index++;
        }
        else vec.push_back(it->second);
    }

    sort(vec.begin(), vec.end(), cmp_hash());
    
    for (set<int> s_it : vec) {
        set_sum = 0;
        for (int i : s_it) set_sum += i;
        result = (result + (set_sum * counter++)) % (1 << 15);
    }
    return result;
}

int calculate_hash_label_propagation(SolutionRepresentation &sol) {
    int result = 0;
    int set_sum;
    auto& [node_to_move, from, to] = sol.book.b_lp.next_move;
    int counter = 1;
    vector<set<int>> vec;
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        if (it->first == from) {
            set<int> s = it->second;
            auto it_ = s.find(node_to_move);
            if (it_ != s.end()) {
                s.erase(it_);
            }
            vec.push_back(s);
        }
        else if (it->first == to) {
            set<int> s = it->second;
            s.insert(node_to_move);
            vec.push_back(s);
        }
        else vec.push_back(it->second);
    }

    sort(vec.begin(), vec.end(), cmp_hash());

    for (set<int> s_it : vec) {
        set_sum = 0;
        for (int i : s_it) set_sum += i;
        result = (result + (set_sum * counter++)) % (1 << 15);
    }

    return result;
}