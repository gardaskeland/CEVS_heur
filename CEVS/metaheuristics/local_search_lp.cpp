#include "local_search_lp.h"

ShallowSolution local_search_lp(Graph &g, int &num_operations) {
    SolutionRepresentation current_solution = SolutionRepresentation(g.n, num_operations);
    current_solution.initial_solution(g.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    int current_cost = current_solution.cost_solution(g);
    cout << "cost of initial solution: " << current_cost << "\n";
    Bookkeep book = Bookkeep(num_operations);
    int best_cost = current_cost;
    cout << "Starting local_search_lp\n";
    int new_cost;

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 30; j++) {
            current_cost += random_choice_add_node(g, current_solution, book);
            set<int> indices = current_solution.get_set_indices_as_set();
            if (!(current_solution.book.b_add_node.v == -1) &&
                indices.find(current_solution.book.b_add_node.si) != indices.end()) {
                current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
            }
        }
        for (int j = 0; j < 50; j++) {
            current_cost += label_propagation_round(g, current_solution);
        }
        remove_nodes(g, current_solution);
        //current_solution.print_solution();
    }
    return ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
}