#include "simulated_annealing.h"

ShallowSolution simulated_annealing(Graph &g, int &num_operations) {

    SolutionRepresentation current_solution = SolutionRepresentation(num_operations);
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    current_solution.initial_solution_max_degree(wg);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    int current_cost = current_solution.cost_solution(wg);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;
    int weights[3] = {33, 33, 34}; // should sum to 100.
    int choice;
    Bookkeep book(num_operations);
    cout << "Starting simulated annealing\n";
    int new_cost;
    vector<int> choices;
    int sol_diff = 0;
    double t_max = 50;
    double t = t_max;
    double alpha = pow(1/t_max, 1.0/num_operations);

    for (int i = 0; i < num_operations; i++) {
        t *= alpha; //t_max * (1 - ((static_cast<float>(i + 1))/num_operations));
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

        
        if (new_cost <= current_cost || rand() < 100 * exp(-(new_cost - current_cost)/t)) {
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
        
        if (current_cost - current_solution.cost_solution(wg) != sol_diff) {
            sol_diff = current_cost - current_solution.cost_solution(wg);
            cout << "Change in sol_diff after operation " << choice << "\n";
            cout << "Current cost: " << current_cost << "\n";
            cout << "sol_diff: " << sol_diff << "\n";
        }
        
        
        

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