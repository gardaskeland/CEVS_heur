#include "alns2.h"

LoggingSolution alns2(Graph &g, LoggingSolution &log_sol, int &num_operations) {
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    SolutionRepresentation current_solution = SolutionRepresentation(wg.n, num_operations);
    current_solution.initial_solution(wg.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    ShallowSolution last_solution;
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
    vector<double> operation_score(operations, 0);
    vector<double> time_taken(operations*2, 0);
    set<int> solution_hashes;
    int change_weights_after = 300;
    int change_weights_count = 0;
    double rate = 0.5;
    int start_score = 20;

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
    double alpha = pow(0.001/t_max, 1.0/num_operations);
    double find_prob_of_acceptance;
    int positive_delta_counter = 0;
    int sum_delta = 0;
    int end_warmup = 1000;

    int escape_counter = 0;
    const int escape_threshold = 1000;

    optional<int> res;

    for (int i = 0; i < num_operations; i++) {
        escape_counter += 1;
        //cout << i << "\n";
        solution_cost_iteration.push_back(current_cost);
        //cout << "11\n";
        last_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        //cout << "22\n";
        t *= alpha; //t_max * (1 - ((static_cast<float>(i + 1))/num_operations));

        if (i == end_warmup && positive_delta_counter > 0) {
            t = -((double)sum_delta / (double)positive_delta_counter)* (1/log(0.8));
            alpha = pow(0.02/t, 1.0/(num_operations-end_warmup));
            cout << "t set to " << t << "\n";
        }
        /**
        if (i % 500 == 499) {
            for (int j = 0; j < 3; j++) {current_cost += label_propagation_round(wg, current_solution);}
            current_cost += remove_nodes_(wg, current_solution);
            if (current_cost < best_cost) {
                best_cost = current_cost;
                last_iteration_of_best_solution = i;
                best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
            }
        }*/
        
        if (escape_counter >= escape_threshold) {
            //cout << "Escape!\n";
            //cout << "Before escape: \n";
            //cout << "Cost: " << current_cost << "\n";
            //current_solution.print_solution();
            /**
            for (int i = 0; i < wg.n / 4; i++) {
                res = add_node_to_set(wg, current_solution);
                new_cost = current_cost + res.value_or(0);
                if (res.has_value()) {
                    current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
                    current_cost = new_cost;
                }
            }*/

            current_cost += add_node_to_all(wg, current_solution) + label_propagation_round(wg, current_solution) \
                + remove_nodes_(wg, current_solution);
            //cout << "After escape: \n";
            //cout << "Cost: " << current_cost << "\n";
            //current_solution.print_solution();

            escape_counter = 0;
            if (current_cost < best_cost) {
                best_cost = current_cost;
                best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters()); 
            }
            current_solution.book.operation_number += 1;
            choices.push_back(operations);
            change_weights_count += 1;

            if (i == num_operations - 1) {
                solution_cost_iteration.push_back(current_cost);
            }
            continue;
        }

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
        
        if (r < c_weights[0]) {
            chrono::steady_clock::time_point begin_0 = chrono::steady_clock::now();
            choice = 0;
            //res0 = random_choice_add_node(wg, current_solution);
            res = add_node_to_neighbours_accept(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            current_solution.book.b_add_node.last_add_operation = i;
            chrono::steady_clock::time_point end_0 = chrono::steady_clock::now();
            time_taken[0] += chrono::duration_cast<chrono::microseconds>(end_0 - begin_0).count();
        } /**else if (r < c_weights[1]) {
            chrono::steady_clock::time_point begin_1 = chrono::steady_clock::now();
            choice = 1;
            //cout << "choice: " << choice << "\n";
            //current_solution.print_solution();
            //Look for better random choice!
            res = weighted_random_split(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            current_solution.book.b_split.last_split_operation = i;
            chrono::steady_clock::time_point end_1 = chrono::steady_clock::now();
            time_taken[1] += chrono::duration_cast<chrono::microseconds>(end_1 - begin_1).count();
            //cout << "current cost - new cost: " << current_cost - new_cost << "\n";
        }   else if (r < c_weights[2]) {
            chrono::steady_clock::time_point begin_2 = chrono::steady_clock::now();
            choice = 2;
            res = add_trio(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            chrono::steady_clock::time_point end_2 = chrono::steady_clock::now();
            time_taken[2] += chrono::duration_cast<chrono::microseconds>(end_2 - begin_2).count();
        } */else if ( r < c_weights[1]) {
            chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
            choice = 1;
            res = label_propagation_accept(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
            time_taken[3] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
        } else if (r < c_weights[2]) {
            chrono::steady_clock::time_point begin_4 = chrono::steady_clock::now();
            choice = 2;
            res = remove_node_accept(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            chrono::steady_clock::time_point end_4 = chrono::steady_clock::now();
            time_taken[4] += chrono::duration_cast<chrono::microseconds>(end_4 - begin_4).count();
        } else if (r < c_weights[3]) {
            chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
            choice = 3;
            res = add_node_to_set(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
            time_taken[5] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
        } /**else if (r < c_weights[4]){
            choice = 4;
            res = swap(wg, current_solution);
            new_cost = current_cost + res.value_or(0);

        }*/ 
        else {//(r < c_weights[4]) {
            choice = 4;
            res = label_propagation_accept_weighted_random(wg, current_solution);
            //cout << "res has value: " << res.has_value() << "\n";
            new_cost = current_cost + res.value_or(0);
        } /**else {
            choice = 5;
            res = fast_merge(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            current_solution.book.b_merge.last_merge_operation = i;
        }*/
        /**else if (r < c_weights[5]) {
            choice = 5;
            res = add_set_over_uncovered(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
        } else {
            choice = 6;
            res = remove_set_op(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
        }*/

            /** 
        else {
            choice = 6;
            res = remove_set_op(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            //current_solution.book.b_merge.last_merge_operation = i;
        }*/
        //cout << "choice: " << choice << "\n";

        if (i >= end_warmup || choice == 5) {
            find_prob_of_acceptance = 100 * exp(-(new_cost - current_cost)/t);
        } else {
            find_prob_of_acceptance = 80;
        }
        
        //results of fast merge not counted here because of volatility.
        if (new_cost > current_cost && choice != 5) {
            sum_delta += new_cost - current_cost;
            positive_delta_counter += 1;
        }
        if (new_cost <= current_cost || rand() % 100 < find_prob_of_acceptance) {
            //if (new_cost > current_cost) cout << "find_prob_of_acceptance: " << find_prob_of_acceptance << "\n";
            if (new_cost < current_cost) operation_score[choice] += 1;
            if (new_cost < best_cost) operation_score[choice] += 1;
            if (choice == 0) {
                chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
                if (res.has_value()) {
                    for (int s : current_solution.book.b_add_node.sets_to_change) {
                        current_solution.add(current_solution.book.b_add_node.v, s);
                    }
                    current_cost = new_cost;
                }
                chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
                time_taken[operations+1] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
            } else if (false && choice == 1) {
                chrono::steady_clock::time_point begin_4 = chrono::steady_clock::now();
                set<int> indices = current_solution.get_set_indices_as_set();
                //cout << "Before split: " << "\n";
                //current_solution.print_solution();
                if (res.has_value()) {
                    //cout << "set to split: " << current_solution.book.b_split.si << "\n";
                    do_split(current_solution);
                    current_cost = new_cost;
                }
                //cout << "After split:  \n";
                //current_solution.print_solution();
                chrono::steady_clock::time_point end_4 = chrono::steady_clock::now();
                time_taken[operations+2] += chrono::duration_cast<chrono::microseconds>(end_4 - begin_4).count();
            }
            else if (false && choice == 2) {
                chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
                //cout << "line 66: ";
                //current_solution.print_solution();
                if (res.has_value()) {
                    tuple<int, int, int> t = current_solution.book.b_perturbation.to_add;
                    set<int> set_to_add;
                    set_to_add.insert(get<0>(t)); set_to_add.insert(get<1>(t)); set_to_add.insert(get<2>(t));
                    current_solution.add_set(set_to_add);
                    current_cost = new_cost;
                } 
                chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
                time_taken[operations+3] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
            }
            else if (choice == 1 || choice == 4) {
                if (res.has_value()) {
                    tuple<int, int, int> move = current_solution.book.b_lp.next_move;
                    current_solution.remove(get<0>(move), get<1>(move));
                    if (get<2>(move) == -1) {
                        set <int> new_set;
                        new_set.insert(get<0>(move));
                        current_solution.add_set(new_set);
                    }
                    else {
                        current_solution.add(get<0>(move), get<2>(move));
                    }
                    current_cost = new_cost;
                }   
            } else if (choice == 2) {
                if (res.has_value()) {
                    for (int si : current_solution.book.b_add_node.sets_to_change) {
                        current_solution.remove(current_solution.book.b_add_node.v, si);
                    }
                    current_cost = new_cost;
                }
            } else if (choice == 3) {
                if (res.has_value()) {
                    current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
                    current_cost = new_cost;  
                }
            } else if (false && choice == 4) {
                if (res.has_value()) {
                    BPerturbation &bp = current_solution.book.b_perturbation;
                    current_solution.remove(bp.to_swap.first, bp.sets_to_swap.first);
                    current_solution.add(bp.to_swap.first, bp.sets_to_swap.second);
                    current_solution.remove(bp.to_swap.second, bp.sets_to_swap.second);
                    current_solution.add(bp.to_swap.second, bp.sets_to_swap.first);
                    current_cost = new_cost;
                }
            } else if (choice == 5) {
                if (res.has_value()) {
                    do_merge(current_solution);
                    current_cost = new_cost;
                }
            }
             else if (choice == 6) {
                if (res.has_value()) {
                    current_solution.remove_set(current_solution.book.b_perturbation.si_to_remove);
                    current_cost = new_cost;  
                }
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
        //if (choice > 5) {
        //    current_cost = new_cost;
        //}
        //cout << "Line 71: ";
        //}

        //cout << "d\n";
        if (current_cost < best_cost) {
            escape_counter = 0;
            best_cost = current_cost;
            last_iteration_of_best_solution = i;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        }
        
        if (!current_solution.simple_feasibility_check()) cout << "not feasible after operation " << choice << "\n";
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
            //current_solution.print_solution();
        }
        //current_solution.print_solution();
        //cout << "e\n";
        
        /**
        int actual_cost = current_solution.cost_solution(wg);
        if (current_cost - actual_cost != sol_diff) {
            sol_diff = current_cost - actual_cost;
            cout << "Change in sol_diff after operation " << choice << "\n";
            cout << "Current cost: " << current_cost << "\n";
            cout << "Actual cost: " << actual_cost << "\n";
            cout << "sol_diff: " << sol_diff << "\n";
            tuple<int, int, int> next_move = current_solution.book.b_lp.next_move;
            cout << "last operation (node, set, set): " << get<0>(next_move) << ", " << get<1>(next_move) << ", " << get<2>(next_move) << "\n";
            cout << "previous solution: \n";
            last_solution.print_solution();
            cout << "current solution: \n"; 
            current_solution.print_solution();
        }*/
        
        
        
        
        
        

        //Makes the current number of operations executed available in all operations.
        current_solution.book.operation_number += 1;
        choices.push_back(choice);
        change_weights_count += 1; 
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
    vector<int> count_choices(operations + 1, 0);
    for (int x : choices) count_choices[x] += 1;
    /**
    cout << "Average time spent on add_node: " << (time_taken[0] / count_choices[0]) / 1000000 << "\n";
    cout << "Average time spent on adding: " << (time_taken[operations + 1] / count_choices[0]) / 1000000 << "\n";
    cout << "Average time spent on greedy_split: " << (time_taken[1] / count_choices[1]) / 1000000 << "\n";
    cout << "Average time spent on splitting: " << (time_taken[operations + 2] / count_choices[1]) / 1000000 << "\n";
    cout << "Average time spent on merge: " << (time_taken[2] / count_choices[2]) / 1000000 << "\n";
    cout << "Average time spent on merging: " << (time_taken[operations + 3] / count_choices[2]) / 1000000 << "\n";
    cout << "Average time spent on label propagation: " << (time_taken[3] / count_choices[3]) / 1000000 << "\n";
    cout << "Average time spent on remove_nodes_: " << (time_taken[4] / count_choices[4]) / 1000000 << "\n";
    //cout << "Average time spent on add_node_to_all: " << (time_taken[5] / count_choices[5]) / 1000000 << "\n";
    */
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
/**
    //Do a local search on best solution using label propagation round
    cout << "Best solution before final local search: \n";
    best_solution.print_solution();
    cout << "Cost: " << best_cost << "\n";

    SolutionRepresentation sol = SolutionRepresentation(wg.n, num_operations);
    sol.initiate_shallow(best_solution);
    current_cost = best_cost;
    while(true) {
        new_cost = current_cost + label_propagation_round(wg, sol);
        if (new_cost >= current_cost) {
            break;
        }
        current_cost = new_cost;
        if (current_cost < best_cost) {
            best_solution = ShallowSolution(sol.clusters, sol.node_in_clusters);
            best_cost = current_cost;
        }
    }
*/ 

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

LoggingSolution test_label_propagation(Graph &g, LoggingSolution &log_sol, int &num_operations) {
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    SolutionRepresentation current_solution = SolutionRepresentation(wg.n, num_operations);
    current_solution.initial_solution(wg.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    ShallowSolution last_solution;
    int current_cost = current_solution.cost_solution(wg);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;

    const int operations = 1;
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
    int operation_score[operations] = {0};
    vector<double> time_taken(operations*2, 0);
    set<int> solution_hashes;
    int change_weights_after = 100;
    int change_weights_count = 0;
    double rate = 0.5;
    int start_score = 20;

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
    double find_prob_of_acceptance;
    int positive_delta_counter = 0;
    int sum_delta = 0;
    int end_warmup = 200;

    optional<int> res;

    for (int i = 0; i < num_operations; i++) {
        //cout << i << "\n";
        solution_cost_iteration.push_back(current_cost);
        //cout << "11\n";
        last_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        //cout << "22\n";
        t *= alpha; //t_max * (1 - ((static_cast<float>(i + 1))/num_operations));

        if (i == end_warmup && positive_delta_counter > 0) {
            t = -((double)sum_delta / (double)positive_delta_counter)* (1/log(0.8));
            alpha = pow(0.01/t, 1.0/(num_operations-end_warmup));
            cout << "t set to " << t << "\n";
        }

        if (i % 100 == 99) {
            current_cost += remove_nodes_(wg, current_solution);
        }

        //current_solution.print_solution();
        //cout << "a\n";
        //cout << "r = " << r << "\n";


        if (i % 4 == 0) {
            chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
            choice = 3;
            res = label_propagation_accept(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
            chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
        } else if (i % 4 == 1) {
            choice = 4;
            res = remove_node_accept(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
        }
        else if (i % 4 == 2) {
            choice = 0;
            res = add_node_to_neighbours_accept(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
        } else {
            choice = 5;
            res = add_node_to_set(wg, current_solution);
            new_cost = current_cost + res.value_or(0);
        }
        //time_taken[3] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
        
        //cout << "choice: " << choice << "\n";

        if (i >= end_warmup) {
            find_prob_of_acceptance = 100 * exp(-(new_cost - current_cost)/t);
        } else {
            find_prob_of_acceptance = 80;
        }

        if (new_cost <= current_cost || rand() % 100 < find_prob_of_acceptance) {
            if (new_cost < current_cost) operation_score[choice] += 1;
            if (new_cost < best_cost) operation_score[choice] += 1;
            if (choice == 0) {
                chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
                if (res.has_value()) {
                    for (int s : current_solution.book.b_add_node.sets_to_change) {
                        current_solution.add(current_solution.book.b_add_node.v, s);
                    }
                    if (new_cost > current_cost) {
                        sum_delta += new_cost - current_cost;
                        positive_delta_counter += 1;
                    }
                    current_cost = new_cost;
                }
                chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
                time_taken[operations+1] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
            } else if (choice == 1) {
                chrono::steady_clock::time_point begin_4 = chrono::steady_clock::now();
                set<int> indices = current_solution.get_set_indices_as_set();
                if (res.has_value()) {
                    do_split(current_solution);
                    if (new_cost > current_cost) {
                        sum_delta += new_cost - current_cost;
                        positive_delta_counter += 1;
                    }
                    current_cost = new_cost;
                }
                chrono::steady_clock::time_point end_4 = chrono::steady_clock::now();
                time_taken[operations+2] += chrono::duration_cast<chrono::microseconds>(end_4 - begin_4).count();
            }
            else if (choice == 2) {
                chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
                //cout << "line 66: ";
                //current_solution.print_solution();
                if (res.has_value()) {
                    do_merge(current_solution);
                    current_cost = new_cost;
                } 
                chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
                time_taken[operations+3] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
            }
            else if (choice == 3) {
                if (res.has_value()) {
                    tuple<int, int, int> move = current_solution.book.b_lp.next_move;
                    current_solution.remove(get<0>(move), get<1>(move));
                    if (get<2>(move) == -1) {
                        set <int> new_set;
                        new_set.insert(get<0>(move));
                        current_solution.add_set(new_set);
                        
                    }
                    else {
                        current_solution.add(get<0>(move), get<2>(move));
                    }
                    if (new_cost > current_cost) {
                        sum_delta += new_cost - current_cost;
                        positive_delta_counter += 1;
                    }
                    current_cost = new_cost;
                }   
            } else if (choice == 4) {
                if (res.has_value()) {
                    for (int si : current_solution.book.b_add_node.sets_to_change) {
                        current_solution.remove(current_solution.book.b_add_node.v, si);
                    }
                    if (new_cost > current_cost) {
                        sum_delta += new_cost - current_cost;
                        positive_delta_counter += 1;
                    }
                    current_cost = new_cost;
                }
            } else if (choice == 5) {
                if (res.has_value()) {
                    current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
                    //cout << "add node to set with cost: " << res.value() << "\n";
                if (new_cost > current_cost) {
                        sum_delta += new_cost - current_cost;
                        positive_delta_counter += 1;
                    }
                    current_cost = new_cost;  
                }
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
        //if (choice == 0) current_cost = new_cost;
        //if (choice > 2 || choice == 0) {
        //    current_cost = new_cost;
        //}
        //cout << "Line 71: ";
        //}

        //cout << "d\n";
        if (current_cost < best_cost) {
            best_cost = current_cost;
            last_iteration_of_best_solution = i;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        }
        
        if (i % 500 == 499) {
            cout << "current cost: " << current_cost << "\n";
        }
        
        /**
        current_solution.print_solution();
        if (!current_solution.verify_co_occurence()) {
            //exit;
        }
        current_solution.print_co_occurence();
        */

        //if (i % 500 == 499) { 
            //cout << "Current cost: " << current_cost << "\n";
            //cout << "Current cost by cost function " << current_solution.cost_solution(g) << "\n";
            //current_solution.print_solution();
        //}
        //current_solution.print_solution();
        //cout << "e\n";
        
        int actual_cost = current_solution.cost_solution(wg);
        if (current_cost - actual_cost != sol_diff) {
            sol_diff = current_cost - actual_cost;
            cout << "Change in sol_diff after operation " << choice << "\n";
            cout << "Current cost: " << current_cost << "\n";
            cout << "Actual cost: " << actual_cost << "\n";
            cout << "sol_diff: " << sol_diff << "\n";
            tuple<int, int, int> next_move = current_solution.book.b_lp.next_move;
            cout << "last operation (node, set, set): " << get<0>(next_move) << ", " << get<1>(next_move) << ", " << get<2>(next_move) << "\n";
            cout << "node to remove: " << current_solution.book.b_add_node.v << "\n";
            cout << "sets to remove from: \n";
            for (int s: current_solution.book.b_add_node.sets_to_change) {
                cout << s << " ";
            }
            cout << "\n";
            cout << "previous solution: \n";
            last_solution.print_solution();
            cout << "current solution: \n"; 
            current_solution.print_solution();
        }
        
        
        
        
        
        

        //Makes the current number of operations executed available in all operations.
        current_solution.book.operation_number += 1;
        choices.push_back(choice);
        change_weights_count += 1; 
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
    /**
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
    //cout << "Average time spent on add_node_to_all: " << (time_taken[5] / count_choices[5]) / 1000000 << "\n";
    */
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
/**
    //Do a local search on best solution using label propagation round
    cout << "Best solution before final local search: \n";
    best_solution.print_solution();
    cout << "Cost: " << best_cost << "\n";

    SolutionRepresentation sol = SolutionRepresentation(wg.n, num_operations);
    sol.initiate_shallow(best_solution);
    current_cost = best_cost;
    while(true) {
        new_cost = current_cost + label_propagation_round(wg, sol);
        if (new_cost >= current_cost) {
            break;
        }
        current_cost = new_cost;
        if (current_cost < best_cost) {
            best_solution = ShallowSolution(sol.clusters, sol.node_in_clusters);
            best_cost = current_cost;
        }
    }
*/
    

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