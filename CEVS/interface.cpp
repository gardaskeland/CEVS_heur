#include "interface.h"

int find_next_solution_hash(SolutionRepresentation &sol, int heuristic) {
    int result;
    if (heuristic == 0 || heuristic == 1) {
        result = calculate_hash_add_node_to_neighbours(sol);
    }
    else if (heuristic == 2) {
        result = calculate_hash_add_node_to_set(sol);
    }
    else if (heuristic == 3) {
        result = calculate_hash_remove_node(sol);
    }
    else {
        result = calculate_hash_label_propagation(sol);
    }
    return result;
    
}

auto find_heuristic(Graph &g, SolutionRepresentation &sol, int heuristic) {
    optional<int> res;
    switch(heuristic) {
        case 0: 
            res = add_node_to_neighbours_accept(g, sol);
        case 1:
            res = add_node_to_neighbours_accept_unchanged(g, sol);
        case 2:
            res = add_node_to_set(g, sol);
        case 3:
            res = remove_node_accept(g, sol);
        case 4:
            res = label_propagation_accept(g, sol);
        case 5:
            res = label_propagation_accept_weighted_random(g, sol);
        case 6:
            res = label_propagation_accept_unchanged(g, sol);
    }
    sol.book.operation_number += 1;
    sol.book.last_heuristic = heuristic; //Stores last heuristic so that we can execute it by calling execute_heuristic

    if (res.has_value()) {
        return find_next_solution_hash(sol, heuristic), res.value();
    } else {
        //Bør spørre Jakob hvordan han håndterer caser der operatorene ikke finner en gyldig operasjon
        return sol.solution_hash(), 0;
    }
}

void execute_heuristic(SolutionRepresentation &sol) {
    int heu = sol.book.last_heuristic;
    if (heu == 0 || heu == 1) {
        execute_add_nodes_to_sets(sol);
    }
    else if (heu == 2) {
        execute_add_node(sol);
    }
    else if (heu == 3) {
        execute_remove(sol);
    }
    else {
        execute_label_propagation(sol);
    }
}

