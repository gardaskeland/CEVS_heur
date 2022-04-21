#include "find_solution_hash.h"

int calculate_hash_add_node_to_neighbours(SolutionRepresentation &sol) {
    int result = 0;
    int set_sum;
    int node_to_add = sol.book.b_add_node.v;
    vector<int> &sets = sol.book.b_add_node.sets_to_change;
    sort(sets.begin(), sets.end());
    int index = 0;
    for (map<int, set<int>>::iterator it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        set_sum = 0;
        if (index < sets.size() && it->first == sets[index]) {
            set_sum += node_to_add;
            index++;
        }
        for (int i : it->second) set_sum += i;
        result += (set_sum * it->first) % 1000000;
    }
    return result;
}

int calculate_hash_add_node_to_set(SolutionRepresentation &sol) {
    int result = 0;
    int set_sum;
    for (map<int, set<int>>::iterator it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        set_sum = 0;
        if (it->first == sol.book.b_add_node.si) {
            set_sum += sol.book.b_add_node.v;
        }
        for (int i : it->second) set_sum += i;
        result += (set_sum * it->first) % 1000000;
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
    for (map<int, set<int>>::iterator it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        set_sum = 0;
        if (index < sets.size() && it->first == sets[index]) {
            set_sum -= node_to_remove;
            index++;
        }
        for (int i : it->second) set_sum += i;
        result += (set_sum * it->first) % 1000000;
    }
    return result;
}

int calculate_hash_label_propagation(SolutionRepresentation &sol) {
    int result = 0;
    int set_sum;
    auto& [node_to_move, from, to] = sol.book.b_lp.next_move;
    for (map<int, set<int>>::iterator it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        set_sum = 0;
        if (it->first == from) {
            set_sum -= node_to_move;
        }
        else if (it->first == to) {
            set_sum += node_to_move;
        }
        for (int i : it->second) set_sum += i;
        result += (set_sum * it->first) % 1000000;
    }
    return result;
}