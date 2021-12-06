#include "local_search.h"

ShallowSolution local_search(Graph g, int num_operations) {
    SolutionRepresentation current_solution = SolutionRepresentation(num_operations);
    current_solution.initial_solution(g.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    int current_cost = current_solution.cost_solution(g);
    int best_cost = current_cost;
    int weights[2] = {50, 50}; // should sum to 100.
    int choice;
    Bookkeep book(num_operations);
    //revert_add_node *ran;
    //revert_greedy_merge *rgm;
    cout << "Starting local_search\n";
    int new_cost;
    vector<int> choices;

    for (int i = 0; i < num_operations; i++) {
        int r = rand() % 100;
        if (r < 50) {
            //cout << "do add_node\n";
            choice = 0;
            add_node(g, current_solution, book);
            new_cost = current_solution.cost_solution(g);
        } else {
            choice = 1;
           // cout << "do merge\n";
            new_cost = current_cost + weighted_random_merge(g, current_solution);
            current_solution.book.b_merge.last_merge_operation = i;
        }

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
                //do_revert_merge(current_solution, book);
                //cout << "After demerging: \n";
                //current_solution.print_solution();
            }
        } else {
            current_cost = new_cost;
            if (choice == 1) {
                current_solution.merge(current_solution.book.b_merge.si, current_solution.book.b_merge.sj);
                
            }
        }


        if (current_cost < best_cost) {
            best_cost = new_cost;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        }

        if (i % 500 == 0) cout << "Current cost: " << current_cost << "\n";
        //current_solution.print_solution();

        //Makes the current number of operations executed available in all operations.
        current_solution.book.operation_number += 1;
        choices.push_back(choice);

    }

    return best_solution;

}