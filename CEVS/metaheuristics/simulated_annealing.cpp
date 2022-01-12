#include "simulated_annealing.h"

ShallowSolution simulated_annealing(Graph &g, int &num_operations) {

    SolutionRepresentation current_solution = SolutionRepresentation(g.n, num_operations);
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    current_solution.initial_solution(wg.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    ShallowSolution last_solution;
    int current_cost = current_solution.cost_solution(wg);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;
    int weights[3] = {33, 33, 34}; //sum to 100.
    double time_taken[6] = {0, 0, 0, 0, 0, 0};
    int choice;
    Bookkeep book(num_operations);
    cout << "Starting simulated annealing\n";
    int new_cost;
    vector<int> choices;
    int sol_diff = 0;
    double t_max = 50;
    double t = t_max;
    double alpha = pow(0.01/t_max, 1.0/num_operations);

    for (int i = 0; i < num_operations; i++) {
        last_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        t *= alpha; //t_max * (1 - ((static_cast<float>(i + 1))/num_operations));
        int r = rand() % 100;
        //cout << "r = " << r << "\n";
        if (r < weights[0]) {
            chrono::steady_clock::time_point begin_0 = chrono::steady_clock::now();
            choice = 0;
            new_cost = current_cost + random_choice_add_node(wg, current_solution, book);
            current_solution.book.b_add_node.last_add_operation = i;
            chrono::steady_clock::time_point end_0 = chrono::steady_clock::now();
            time_taken[0] += chrono::duration_cast<chrono::microseconds>(end_0 - begin_0).count();
        } else if (r < weights[0] + weights[1]) {
            chrono::steady_clock::time_point begin_1 = chrono::steady_clock::now();
            choice = 1;
            //cout << "choice: " << choice << "\n";
            //current_solution.print_solution();
            new_cost = current_cost + random_choice_split(wg, current_solution);
            current_solution.book.b_split.last_split_operation = i;
            chrono::steady_clock::time_point end_1 = chrono::steady_clock::now();
            time_taken[1] += chrono::duration_cast<chrono::microseconds>(end_1 - begin_1).count();
            //cout << "current cost - new cost: " << current_cost - new_cost << "\n";
        } else {
            chrono::steady_clock::time_point begin_2 = chrono::steady_clock::now();
            choice = 2;
            new_cost = current_cost + weighted_random_merge(wg, current_solution);
            current_solution.book.b_merge.last_merge_operation = i;
            chrono::steady_clock::time_point end_2 = chrono::steady_clock::now();
            time_taken[2] += chrono::duration_cast<chrono::microseconds>(end_2 - begin_2).count();
        }

        
        if (new_cost <= current_cost || rand() % 100 < 100 * exp(-(new_cost - current_cost)/t)) {
            if (choice == 0) {
                chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
                set<int> indices = current_solution.get_set_indices_as_set();
                if (!(current_solution.book.b_add_node.v == -1) &&
                    indices.find(current_solution.book.b_add_node.si) != indices.end()) {
                    current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
                    current_cost = new_cost;
                }
                chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
                time_taken[3] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
            } else if (choice == 1) {
                chrono::steady_clock::time_point begin_4 = chrono::steady_clock::now();
                set<int> indices = current_solution.get_set_indices_as_set();
                if (!(current_solution.book.b_split.si == -1) &&
                    indices.find(current_solution.book.b_split.si) != indices.end()) {
                    //cout << "before: ";
                    //current_solution.print_solution();
                    pair<set<int>, set<int>> p = current_solution.book.b_split.cut;
                    current_solution.disjoint_split(current_solution.book.b_split.si, p.first, p.second);
                    //cout << "after: ";
                    //current_solution.print_solution();
                    current_cost = new_cost;
                }
                chrono::steady_clock::time_point end_4 = chrono::steady_clock::now();
                time_taken[4] += chrono::duration_cast<chrono::microseconds>(end_4 - begin_4).count();
            }
            else if (choice == 2) {
                chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
                //cout << "line 66: ";
                //current_solution.print_solution();
                current_solution.merge(current_solution.book.b_merge.si, current_solution.book.b_merge.sj);
                current_cost = new_cost;
                chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
                time_taken[5] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
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
        
        /*
        if (current_cost - current_solution.cost_solution(wg) != sol_diff) {
            sol_diff = current_cost - current_solution.cost_solution(wg);
            cout << "Change in sol_diff after operation " << choice << "\n";
            cout << "Current cost: " << current_cost << "\n";
            cout << "sol_diff: " << sol_diff << "\n";
            cout << "last operation (v, set): " << current_solution.book.b_add_node.v << ", " << current_solution.book.b_add_node.si << "\n";
            cout << "previous solution: \n";
            last_solution.print_solution();
            cout << "current solution: \n"; 
            current_solution.print_solution();
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
    int count_choices[3] = {0, 0, 0};
    for (int x : choices) count_choices[x] += 1;
    cout << "Average time spent on add_node: " << (time_taken[0] / count_choices[0]) / 1000000 << "\n";
    cout << "Average time spent on adding: " << (time_taken[3] / count_choices[0]) / 1000000 << "\n";
    cout << "Average time spent on greedy_split: " << (time_taken[1] / count_choices[1]) / 1000000 << "\n";
    cout << "Average time spent on splitting: " << (time_taken[4] / count_choices[1]) / 1000000 << "\n";
    cout << "Average time spent on merge: " << (time_taken[2] / count_choices[2]) / 1000000 << "\n";
    cout << "Average time spent on merging: " << (time_taken[5] / count_choices[2]) / 1000000 << "\n";
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

    //cout << "final cost: " << current_solution.cost_solution(wg) << "\n";

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