#include "local_search.h"

SolutionRepresentation local_search(Graph g) {
    SolutionRepresentation current_solution = SolutionRepresentation();
    current_solution.initial_solution(g.n);
    SolutionRepresentation best_solution = current_solution;
    int current_cost = current_solution.cost_solution(g);
    int best_cost = current_cost;
    int num_operations = 100;
    int weights[2] = {50, 50}; // should sum to 100.
    int choice;
    //revert_add_node *ran;
    //revert_greedy_merge *rgm;
    cout << "Starting local_search";

    for (int i = 0; i < num_operations; i++) {
        int r = rand() % 100;
        if (r < 50) {
            cout << "do add_node\n";
            choice = 0;
            add_node(g, current_solution);
        } else {
            choice = 1;
            cout << "do greedy_merge\n";
            greedy_merge(g, current_solution);
        }

        int new_cost = current_solution.cost_solution(g);
        cout << "New cost: " << new_cost << "\n";
        if (new_cost < best_cost) {
            best_cost = new_cost;
            best_solution = current_solution.copy_solution();
        }
        current_solution.print_solution();
    }

    return best_solution;

}