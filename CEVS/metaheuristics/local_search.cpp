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
    Bookkeep book;
    //revert_add_node *ran;
    //revert_greedy_merge *rgm;
    cout << "Starting local_search\n";

    for (int i = 0; i < num_operations; i++) {
        int r = rand() % 100;
        if (r < 50) {
            //cout << "do add_node\n";
            choice = 0;
            add_node(g, current_solution, book);
        } else {
            choice = 1;
           // cout << "do merge\n";
            weighted_random_merge(g, current_solution, book);
        }

        int new_cost = current_solution.cost_solution(g);
        //cout << "New cost: " << new_cost << "\n";

        //We revert operations in here
        if (new_cost > current_cost) {
            if (choice == 0) {
                //cout << "Removing node from si. Before: \n";
               // current_solution.print_solution();
                do_revert_add_node(current_solution, book);
               // cout << "After: \n";
               // current_solution.print_solution();
            }

            if (choice == 1) {
                //cout << "Demerging:\n";
                //current_solution.print_solution();
                do_revert_merge(current_solution, book);
                //cout << "After demerging: \n";
                //current_solution.print_solution();
            }
        } else {
            current_cost = new_cost;
        }


        if (new_cost < best_cost) {
            best_cost = new_cost;
            best_solution = current_solution.copy_solution();
        }

        cout << "Current cost: " << current_cost << "\n";
        //current_solution.print_solution();
    }

    return best_solution;

}