#include "local_search.h"

ShallowSolution local_search(Graph g, int num_operations) {
    SolutionRepresentation current_solution = SolutionRepresentation(num_operations);
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

        if (i % 10 == 9) {
            cout << i << "\n";
        }

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

ShallowSolution local_search_on_cc(Graph g, int num_operations) {

    SolutionRepresentation current_solution = SolutionRepresentation(num_operations);
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    current_solution.initial_solution(wg.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    int current_cost = current_solution.cost_solution(wg);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;
    int weights[2] = {50, 50}; // should sum to 100.
    int choice;
    Bookkeep book(num_operations);
    cout << "Starting local_search\n";
    int new_cost;
    vector<int> choices;
    int sol_diff = 0;

    for (int i = 0; i < num_operations; i++) {
        int r = rand() % 100;
        if (r < 50) {
            choice = 0;
            new_cost = current_cost + add_node(wg, current_solution, book);
            current_solution.book.b_add_node.last_add_operation = i;
        } else {
            choice = 1;
            new_cost = current_cost + weighted_random_merge(wg, current_solution);
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

        if (i % 10 == 9) {
            cout << i << "\n";
        }

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
    //The result is not quite the same. Must debug.
    cout << "final cost: " << current_solution.cost_solution(wg) << "\n";
    ShallowSolution to_return;
    set<int> next_cluster;
    vector<set<int>> node_to_cluster = vector<set<int>>(g.n, set<int>());
    int counter = 0;
    //cout << "ok0\n";
    for (map<int, set<int>>::iterator it = best_solution.clusters.begin(); it != best_solution.clusters.end(); it++) {
        next_cluster = set<int>();
        for (int i : it->second) {
            for (int j : revert.other_cc[i]) {
                next_cluster.insert(j);
                node_to_cluster[j].insert(counter);
            }
        }
        to_return.clusters[counter] = next_cluster;
        counter += 1;
    }
    //cout << "ok1\n";
    for (set<int> s : revert.isolated_cc) {
        to_return.clusters[counter] = s;
        for (int i : s) {
            node_to_cluster[i].insert(counter);
        }
        counter += 1;
    }
    //cout << "ok2\n";
    for (int i = 0; i < node_to_cluster.size(); i++) {
        to_return.node_in_clusters[i] = node_to_cluster[i];
    }

    //print out other_cc
    int c = 0;
    for (set<int> s : revert.other_cc) {
        cout << "node " << c << " is mapped to: ";
        for (int i : s) {
            cout << i << ", ";
        }
        cout << "\n";
        c += 1;
    }

    //cout << "ok\n";
    return to_return;

}