#include "local_search.h"

ShallowSolution local_search(Graph &g, int &num_operations) {
    SolutionRepresentation current_solution = SolutionRepresentation(g.n, num_operations);
    current_solution.initial_solution(g.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    int current_cost = current_solution.cost_solution(g);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;
    int weights[2] = {50, 50}; // should sum to 100.
    int choice;
    Bookkeep book(num_operations);
    //revert_add_node *ran;
    //revert_greedy_merge *rgm;
    cout << "Starting local_search\n";
    int new_cost;
    vector<int> choices;
    int sol_diff = 0;

    for (int i = 0; i < num_operations; i++) {
        //cout << "i\n";
        int r = rand() % 100;
        if (r < 50) {
            //cout << "do add_node\n";
            choice = 0;
            new_cost = current_cost + add_node(g, current_solution, book);
            current_solution.book.b_add_node.last_add_operation = i;
            //cout << "add\n";
        } else {
            choice = 1;
            //cout << "do merge\n";
            new_cost = current_cost + weighted_random_merge(g, current_solution);
            current_solution.book.b_merge.last_merge_operation = i;
        }
        //cout << "line 34: ";
        //current_solution.print_solution();

        //cout << "New cost: " << new_cost << "\n";

        //We revert operations in here
        /**
        if (new_cost > current_cost) {
            if (choice == 0) {
                //cout << "Removing node from si. Before: \n";
               // current_solution.print_solution();
                //do_revert_add_node(current_solution, book);
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
            */
        //} else {
        if (new_cost <= current_cost) {
            current_cost = new_cost;
            if (choice == 0) {
                if (!(current_solution.book.b_add_node.v == -1) &&
                    current_solution.get_set_indices_as_set().find(current_solution.book.b_add_node.si) != current_solution.get_set_indices_as_set().end()) {
                    current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
                }
            }
            else if (choice == 1) {
                //cout << "line 66: ";
                //current_solution.print_solution();
                current_solution.merge(current_solution.book.b_merge.si, current_solution.book.b_merge.sj);
            }     
        }
        //cout << "Line 71: ";
        //current_solution.print_solution();
        //}


        if (current_cost < best_cost) {
            best_cost = current_cost;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        }
        /*
        if (i % 10 == 9) {
            cout << i << "\n";
        }
        */

        if (i % 500 == 499) { 
            cout << "Current cost: " << current_cost << "\n";
            //cout << "Current cost by cost function " << current_solution.cost_solution(g) << "\n";
            current_solution.print_solution();
        }
        /**
        if (current_cost - current_solution.cost_solution(g) != sol_diff) {
            sol_diff = current_cost - current_solution.cost_solution(g);
            cout << "Change in sol_diff after operation " << choice << "\n";
            cout << "Current cost: " << current_cost << "\n";
            cout << "sol_diff: " << sol_diff << "\n";
        }
        */
        

        //Makes the current number of operations executed available in all operations.
        current_solution.book.operation_number += 1;
        choices.push_back(choice);
        //cout << "line 95: ";
        //current_solution.print_solution();
        /**
        for (map<int, set<int>>::iterator it = current_solution.get_clusters().begin(); it != current_solution.get_clusters().end(); it++) {
            if ((it->second).size() == 0) {
                cout << "After operation " << choice << " we have a zero set in the solution.\n";
                exit(0);
            }
        }
        */
        

    }

    return best_solution;

}
//TODO: split seems to always give a worse solution at the moment. So it is never chosen.
//Also need to make a better cost calculation for split: Does not consider that an edge
//can be accounted for by another cluster at the moment.
ShallowSolution local_search_on_cc(Graph &g, int &num_operations) {

    SolutionRepresentation current_solution = SolutionRepresentation(g.n, num_operations);
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    current_solution.initial_solution_complete_graph(wg.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    int current_cost = current_solution.cost_solution(wg);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;
    int weights[3] = {33, 33, 34}; // should sum to 100.
    int choice;
    Bookkeep book(num_operations);
    cout << "Starting local_search\n";
    int new_cost;
    vector<int> choices;
    int sol_diff = 0;

    for (int i = 0; i < num_operations; i++) {
        int r = rand() % 100;
        //cout << "r = " << r << "\n";
        if (r < weights[0]) {
            choice = 0;
            new_cost = current_cost + add_node(wg, current_solution, book);
            current_solution.book.b_add_node.last_add_operation = i;
        } else if (r < weights[0] + weights[1]) {
            choice = 1;
            //cout << "choice: " << choice << "\n";
            new_cost = current_cost + greedy_split(wg, current_solution);
            current_solution.book.b_split.last_split_operation = i;
            //cout << "current cost - new cost: " << current_cost - new_cost << "\n";
        } else {
            choice = 2;
            new_cost = current_cost + weighted_random_merge(wg, current_solution);
            current_solution.book.b_merge.last_merge_operation = i;
        }
        //cout << "line 34: ";
        //current_solution.print_solution();
        //cout << "choice: " << choice << "\n";
        //cout << "New cost: " << new_cost << "\n";

        //We revert operations in here
        /**
        if (new_cost > current_cost) {
            if (choice == 0) {
                //cout << "Removing node from si. Before: \n";
               // current_solution.print_solution();
                //do_revert_add_node(current_solution, book);
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
            */
        //} else {
        if (new_cost <= current_cost) {
            current_cost = new_cost;
            if (choice == 0) {
                if (!(current_solution.book.b_add_node.v == -1) &&
                    current_solution.get_set_indices_as_set().find(current_solution.book.b_add_node.si) != current_solution.get_set_indices_as_set().end()) {
                    current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
                }
            } else if (choice == 1) {
                set<int> indices = current_solution.get_set_indices_as_set();
                if (!(current_solution.book.b_split.si == -1) &&
                    indices.find(current_solution.book.b_split.si) != indices.end()) {
                    //cout << "before: ";
                    //current_solution.print_solution();
                    pair<set<int>, set<int>> p = current_solution.book.b_split.cut;
                    current_solution.disjoint_split(current_solution.book.b_split.si, p.first, p.second);
                    //cout << "after: ";
                    //current_solution.print_solution();
                }
            }
            else if (choice == 2) {
                //cout << "line 66: ";
                //current_solution.print_solution();
                current_solution.merge(current_solution.book.b_merge.si, current_solution.book.b_merge.sj);
            }     
        }
        //cout << "Line 71: ";
        //}


        if (current_cost < best_cost) {
            best_cost = current_cost;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        }
        
        //if (i % 10 == 9) {
        //    cout << i << "\n";
        //}
        

        if (i % 500 == 499) { 
            cout << "Current cost: " << current_cost << "\n";
            //cout << "Current cost by cost function " << current_solution.cost_solution(g) << "\n";
            current_solution.print_solution();
        }
        /**
        if (current_cost - current_solution.cost_solution(wg) != sol_diff) {
            sol_diff = current_cost - current_solution.cost_solution(wg);
            cout << "Change in sol_diff after operation " << choice << "\n";
            cout << "Current cost: " << current_cost << "\n";
            cout << "sol_diff: " << sol_diff << "\n";
        }
        */
        
        

        //Makes the current number of operations executed available in all operations.
        current_solution.book.operation_number += 1;
        choices.push_back(choice);
        //cout << "line 95: ";
        //cout <<"current cost: " << current_cost << "\ncurrent solution: ";
        //current_solution.print_solution();
        /**
        for (map<int, set<int>>::iterator it = current_solution.get_clusters().begin(); it != current_solution.get_clusters().end(); it++) {
            if ((it->second).size() == 0) {
                cout << "After operation " << choice << " we have a zero set in the solution.\n";
                exit(0);
            }
        }
        */
        

    }
    /**
    cout << "edges:\n";
    for (int i = 0; i < wg.n; i++) {
        for (int j : wg.adj[i]) {
            cout << i << " " << j << " with weight " << wg.get_edge_cost(i, j) << "\n";
        }
    }
    for (int i = 0; i < wg.n; i++) {
        cout << "node weight of " << i << ": " << wg.get_node_weight(i) << "\n";
    }
    */

    cout << "final cost: " << current_solution.cost_solution(wg) << "\n";

    //print out other_cc
    /*
    int c = 0;
    for (set<int> s : revert.other_cc) {
        cout << "node " << c << " is mapped to: ";
        for (int i : s) {
            cout << i << ", ";
        }
        cout << "\n";
        c += 1;
    }*/

    //cout << "ok\n";
    return from_cc_sol_to_sol(g, best_solution, revert);
}