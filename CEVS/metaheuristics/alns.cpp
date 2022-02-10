#include "alns.h"

LoggingSolution alns(Graph &g, LoggingSolution &log_sol, int &num_operations) {
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    SolutionRepresentation current_solution = SolutionRepresentation(wg.n, num_operations);
    current_solution.initial_solution(wg.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    //ShallowSolution last_solution;
    int current_cost = current_solution.cost_solution(wg);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;

    const int operations = 5;
    double start_weight = 100 / operations;
    vector<double> weights(operations, start_weight);
    vector<double> c_weights(operations, 0);
    for (int i = 0; i < operations; i++) {
        if (i == 0) {
            c_weights[i] = weights[i];
            continue;
        }
        c_weights[i] = c_weights[i - 1] + weights[i];
    }
    int operation_score[operations] = {0, 0, 0, 0, 0};
    vector<double> time_taken(operations*2, 0);
    set<int> solution_hashes;
    int change_weights_after = 100;
    int change_weights_count = 0;
    double rate = 0.5;
    int start_score = 20;

    //set to 0 when reaching escape threshold or new best solution is found. +1 each iteration.
    int escape_counter = 0;
    int escape_threshold = 5;

    //cost at start of iteration
    vector<int> solution_cost_iteration;
    //num of operator executed at iteration
    int last_iteration_of_best_solution = 0;
    vector<vector<double>> weights_over_iteration;
    weights_over_iteration.push_back(weights);

    int choice;
    //Bookkeep book(num_operations);
    cout << "Starting alns\n";
    int new_cost;
    vector<int> choices;
    int sol_diff = 0;
    double t_max = 50;
    double t = t_max;
    double alpha = pow(0.01/t_max, 1.0/num_operations);
    int start_index = 0;
    if (current_solution.num_sets() == 0) {
        start_index = num_operations;
        cout << "Solved by kernel, expext irregular behaviour.\n";
    }

    for (int i = start_index; i < num_operations; i++) {
        //cout << i << "\n";
        solution_cost_iteration.push_back(current_cost);
        //cout << "11\n";
        //last_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        //cout << "22\n";
        t *= alpha; //t_max * (1 - ((static_cast<float>(i + 1))/num_operations));

        if (change_weights_count >= change_weights_after) {
            change_weights_count = 0;
            int max_score = 0;
            int max_ind = 0;
            double total_score = 0;
            for (int i = 0; i < operations; i++) {
                total_score += operation_score[i];
            }
            vector<pair<int, int>> give_points_to;
            for (int i = 0; i < operations; i++) {
                if (operation_score[i] > max_score) {
                    max_score = operation_score[i];
                    max_ind = i;
                }
                if (operation_score[i] / total_score < 0.1) {
                    give_points_to.push_back(pair<int, int>(i, floor(total_score * 0.1 - operation_score[i])));
                }
            }
            for (pair<int, int> p : give_points_to) {
                operation_score[p.first] += p.second;
                operation_score[max_ind] -= p.second;
            }
            //cout << "recalibrate weights: \n";
            for (int i = 0; i < operations; i++) {
                weights[i] = (1 - rate) * weights[i] + rate * 100 * (operation_score[i] / total_score);
                operation_score[i] = start_score;
                //cout << i << ": " << weights[i] << "\n";
            }
            weights_over_iteration.push_back(weights);
            for (int i = 0; i < operations; i++) {
                if (i == 0) {
                    c_weights[i] = weights[i];
                    continue;
                }
                c_weights[i] = c_weights[i - 1] + weights[i];
            }
        }
        
        //current_solution.print_solution();
        int r = rand() % 100;
        //cout << "a\n";
        //cout << "r = " << r << "\n";
        optional<int> res0, res1, res2;
        if (escape_counter >= escape_threshold) {
            escape_counter = 0;
            chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
            choice = 5;
            new_cost = current_cost + add_node_to_all(wg, current_solution);
            chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
            time_taken[5] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
        }
        else if (r < c_weights[0]) {
            chrono::steady_clock::time_point begin_0 = chrono::steady_clock::now();
            choice = 0;
            res0 = random_choice_add_node(wg, current_solution);
            new_cost = current_cost + res0.value_or(0);
            current_solution.book.b_add_node.last_add_operation = i;
            chrono::steady_clock::time_point end_0 = chrono::steady_clock::now();
            time_taken[0] += chrono::duration_cast<chrono::microseconds>(end_0 - begin_0).count();
        } else if (r < c_weights[1]) {
            chrono::steady_clock::time_point begin_1 = chrono::steady_clock::now();
            choice = 1;
            //cout << "choice: " << choice << "\n";
            //current_solution.print_solution();
            //Look for better random choice!
            res1 = random_choice_split(wg, current_solution);
            new_cost = current_cost + res1.value_or(0);
            current_solution.book.b_split.last_split_operation = i;
            chrono::steady_clock::time_point end_1 = chrono::steady_clock::now();
            time_taken[1] += chrono::duration_cast<chrono::microseconds>(end_1 - begin_1).count();
            //cout << "current cost - new cost: " << current_cost - new_cost << "\n";
        } else if (r < c_weights[2]) {
            chrono::steady_clock::time_point begin_2 = chrono::steady_clock::now();
            choice = 2;
            res2 = weighted_random_merge(wg, current_solution);
            new_cost = current_cost + res2.value_or(0);
            current_solution.book.b_merge.last_merge_operation = i;
            chrono::steady_clock::time_point end_2 = chrono::steady_clock::now();
            time_taken[2] += chrono::duration_cast<chrono::microseconds>(end_2 - begin_2).count();
        } else if ( r < c_weights[3]) {
            chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
            choice = 3;
            new_cost = current_cost + label_propagation_round(wg, current_solution);
            chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
            time_taken[3] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
        } else {//if (r < c_weights[4]) {
            chrono::steady_clock::time_point begin_4 = chrono::steady_clock::now();
            choice = 4;
            new_cost = current_cost + remove_nodes_(wg, current_solution);
            chrono::steady_clock::time_point end_4 = chrono::steady_clock::now();
            time_taken[4] += chrono::duration_cast<chrono::microseconds>(end_4 - begin_4).count();
        }/**
        else {
            chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
            choice = 5;
            new_cost = current_cost + add_node_to_all(wg, current_solution);//add_node_to_percent(wg, current_solution, min(5*(t/t_max)*100, 100.0));
            chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
            time_taken[5] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
        }
        */
        //cout << "choice: " << choice << "\n";

        
        if (new_cost <= current_cost || rand() % 100 < 100 * exp(-(new_cost - current_cost)/t)) {
            if (new_cost < current_cost) operation_score[choice] += 1;
            if (new_cost < best_cost) operation_score[choice] += 1;
            if (choice == 0) {
                chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
                if (res0.has_value()) {
                    do_add(current_solution);
                    current_cost = new_cost;
                }
                chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
                time_taken[operations+1] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
            } else if (choice == 1) {
                chrono::steady_clock::time_point begin_4 = chrono::steady_clock::now();
                set<int> indices = current_solution.get_set_indices_as_set();
                if (res1.has_value()) {
                    do_split(current_solution);
                    current_cost = new_cost;
                }
                chrono::steady_clock::time_point end_4 = chrono::steady_clock::now();
                time_taken[operations+2] += chrono::duration_cast<chrono::microseconds>(end_4 - begin_4).count();
            }
            else if (choice == 2) {
                chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
                //cout << "line 66: ";
                //current_solution.print_solution();
                if (res2.has_value()) {
                    do_merge(current_solution);
                    current_cost = new_cost;
                } 
                chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
                time_taken[operations+3] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
            } else {
                current_cost = new_cost;
            }
        }
        //cout << "c\n";

        int solution_hash = current_solution.solution_hash();
        if (!(solution_hashes.find(solution_hash) != solution_hashes.end())) {
            //cout << "cc/\n";
            operation_score[choice] += 1;
            solution_hashes.insert(solution_hash);
        }
        //cout << "ccc\n";
        if (choice > 2) {
            current_cost = new_cost;
        }
        //cout << "Line 71: ";
        //}

        //cout << "d\n";
        if (current_cost < best_cost) {
            best_cost = current_cost;
            last_iteration_of_best_solution = i;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
            escape_counter = 0;
        }
        
        //if (i % 10 == 9) {
        //    cout << i << "\n";
        //}
        /**
        current_solution.print_solution();
        if (!current_solution.verify_co_occurence()) {
            //exit;
        }
        current_solution.print_co_occurence();
        */

        if (i % 500 == 499) { 
            cout << "Current cost: " << current_cost << "\n";
            //cout << "Current cost by cost function " << current_solution.cost_solution(g) << "\n";
            current_solution.print_solution();
        }
        //current_solution.print_solution();
        //cout << "e\n";
        
        /**
        if (current_cost - current_solution.cost_solution(wg) != sol_diff) {
            sol_diff = current_cost - current_solution.cost_solution(wg);
            cout << "Change in sol_diff after operation " << choice << "\n";
            cout << "Current cost: " << current_cost << "\n";
            cout << "sol_diff: " << sol_diff << "\n";
            cout << "last operation (set, set): " << current_solution.book.b_merge.si << ", " << current_solution.book.b_merge.sj << "\n";
            cout << "previous solution: \n";
            last_solution.print_solution();
            cout << "current solution: \n"; 
            current_solution.print_solution();
        }*/
        
        
        
        
        
        

        //Makes the current number of operations executed available in all operations.
        current_solution.book.operation_number += 1;
        choices.push_back(choice);
        change_weights_count += 1;
        escape_counter += 1;
        if (i == num_operations - 1) {
            solution_cost_iteration.push_back(current_cost);
        }
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
    int count_choices[6] = {0, 0, 0, 0, 0, 0};
    for (int x : choices) count_choices[x] += 1;
    cout << "Average time spent on add_node: " << (time_taken[0] / count_choices[0]) / 1000000 << "\n";
    cout << "Average time spent on adding: " << (time_taken[operations + 1] / count_choices[0]) / 1000000 << "\n";
    cout << "Average time spent on greedy_split: " << (time_taken[1] / count_choices[1]) / 1000000 << "\n";
    cout << "Average time spent on splitting: " << (time_taken[operations + 2] / count_choices[1]) / 1000000 << "\n";
    cout << "Average time spent on merge: " << (time_taken[2] / count_choices[2]) / 1000000 << "\n";
    cout << "Average time spent on merging: " << (time_taken[operations + 3] / count_choices[2]) / 1000000 << "\n";
    cout << "Average time spent on label propagation: " << (time_taken[3] / count_choices[3]) / 1000000 << "\n";
    cout << "Average time spent on remove_nodes_: " << (time_taken[4] / count_choices[4]) / 1000000 << "\n";
    cout << "Average time spent on add_node_to_all: " << (time_taken[5] / count_choices[5]) / 1000000 << "\n";
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
    ShallowSolution shallow = from_cc_sol_to_sol(g, best_solution, revert);
    for (map<int, set<int>>::iterator it = shallow.clusters.begin(); it != shallow.clusters.end(); it++) {
        log_sol.clusters[it->first] = it->second;
    }
    for (map<int, set<int>>::iterator it = shallow.node_in_clusters.begin(); it != shallow.node_in_clusters.end(); it++) {
        log_sol.node_in_clusters[it->first] = it->second;
    }
    log_sol.change_weights_after = change_weights_after;
    log_sol.operator_iteration = move(choices);
    //choices.clear();
    log_sol.solution_cost_iteration = move(solution_cost_iteration);
    //solution_cost_iteration.clear();
    log_sol.last_iteration_of_best_solution = last_iteration_of_best_solution;
    log_sol.time_taken = move(time_taken);
    log_sol.weights_over_iteration = weights_over_iteration;
    log_sol.num_operations = num_operations;
    
    return log_sol;


}