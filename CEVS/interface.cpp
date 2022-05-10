#include "interface.h"

int find_next_solution_hash(SolutionRepresentation &sol, int heuristic) {
    //cout << "finding solution hash of " << heuristic << "+\n";
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
    else if (4 <= heuristic && heuristic <= 6) {
        result = calculate_hash_label_propagation(sol);
    }
    else if (heuristic == 7) {
        result = sol.solution_hash();
    }
    else if (heuristic == 8) {
        result = sol.book.b_remove_add.solution_hash;
    }
    return result;
    
}

pair<int, int> find_heuristic(Graph &g, SolutionRepresentation &sol, int heuristic) {

    //for debug
    //if (sol.book.operation_number % 1 == 0) {
    //    cout << "Current cost: " << sol.cost_solution(g) << "\n";
    //}

    optional<int> res = {};

    if (heuristic == 0)
        res = add_node_to_neighbours_accept(g, sol);
    else if (heuristic == 1)
        res = add_node_to_neighbours_accept_unchanged(g, sol);
    else if (heuristic == 2)
        res = add_node_to_set(g, sol);
    else if (heuristic == 3) {
        //cout << "calling remove node accept\n";
        res = remove_node_accept(g, sol);
    }
    else if (heuristic == 4)
        res = label_propagation_accept(g, sol);
    else if (heuristic == 5)
        res = label_propagation_accept_weighted_random(g, sol);
    else if (heuristic == 6)
        res = label_propagation_accept_unchanged(g, sol);
    else if (heuristic == 7)
        res = escape_by_add_lp(g, sol, 20);
    else if (heuristic == 8)
        res = sample_remove_add_3(g, sol);

    //cout << "found operator\n";

    if (heuristic > 8) {
        cout << "OOPS! Heuristic is " << heuristic << "\n";
    }
    sol.book.operation_number += 1;
    sol.book.last_heuristic = heuristic; //Stores last heuristic so that we can execute it by calling execute_heuristic

    if (res.has_value()) {
        //cout << "res.value(): " << res.value() << "\n";
        return make_pair(find_next_solution_hash(sol, heuristic), res.value());
    } else {
        //Bør spørre Jakob hvordan han håndterer caser der operatorene ikke finner en gyldig operasjon
        //cout << "no change: " << 0 << "\n";
        return make_pair(-1, 0);
    }
}

int objective_function(Graph &g, SolutionRepresentation &sol) {
    return sol.book.current_cost = sol.cost_solution(g);
}

void execute_heuristic(SolutionRepresentation &sol) {
    int heu = sol.book.last_heuristic;
    if (heu == 0 || heu == 1) {
        //cout << "add to neighbours\n";
        execute_add_nodes_to_sets(sol);
    }
    else if (heu == 2) {
       // cout << "add node to set\n";
        execute_add_node(sol);
       
    }
    else if (heu == 3) {
       // cout << "remove from set\n";
        execute_remove(sol);
        
    }
    else if (4 <= heu && heu <= 6) {
       // cout << "lp\n";
        execute_label_propagation(sol);  
    }
    else if (heu == 8) {
        execute_remove_add_3(sol);
    }

    //else if (heu == 7) do nothing
}

Graph make_graph_from_gml(string filename) {
    vector<vector<int>> adj = read_gml(filename);
    return Graph(adj);
}

Graph make_graph_from_gr(string filename) {
    vector<vector<int>> adj = read_gz_file(filename);
    return Graph(adj);
}

//TODO: Must write method that reads the nodes.csv file to find how many
//nodes are in the graph
/**
Graph make_graph_from_csv(string filename) {
    vector<vector<int>> adj = read_csv_graph(filename);
    return Graph(adj);
}*/

