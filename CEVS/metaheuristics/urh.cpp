#include "urh.h"

LoggingSolution urh(Graph &input_g, LoggingSolution &log_sol, int num_operations, bool with_cc) {
    RevertKernel revert;
    int temp_n;
    Graph *g;
    if (with_cc) {
        WeightedGraph wg = find_critical_clique_graph(input_g, revert);;
        g = &wg;
    }
    else {
        g = &input_g;
    }
    SolutionRepresentation current_solution = SolutionRepresentation(g->n, num_operations);
    current_solution.initial_solution(g->n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    ShallowSolution last_solution;
    int current_cost = current_solution.cost_solution(*g);
    cout << "cost of initial solution: " << current_cost << "\n";
    int best_cost = current_cost;

    const int operations = 8;
    vector<double> time_taken(operations*2, 0);
    set<int> solution_hashes;

    //cost at start of iteration
    vector<int> solution_cost_iteration;
    //num of operator executed at iteration
    int last_iteration_of_best_solution = 0;

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

    //int escape_counter = 0;
    //const int escape_threshold = 1000;

    bool local_search = false;
    const int activate_local_search = num_operations - (num_operations / 20);

    optional<int> res;

    for (int i = 0; i < num_operations; i++) {
        //escape_counter += 1;
        current_solution.book.operation_number = i;
        //cout << i << "\n";
        solution_cost_iteration.push_back(current_cost);
        //cout << "11\n";
        //last_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        //cout << "22\n";
        t *= alpha; //t_max * (1 - ((static_cast<float>(i + 1))/num_operations));

        if (i == end_warmup && positive_delta_counter > 0) {
            t = -((double)sum_delta / (double)positive_delta_counter)* (1/log(0.8));
            alpha = pow(0.02/t, 1.0/(num_operations-end_warmup));
            cout << "t set to " << t << "\n";
        }

        //current_solution.print_solution();
        int r = current_solution.ra.get_random_int() % operations;
        //cout << "a\n";
        //cout << "r = " << r << "\n";
        
        if (r == 0) {
            //chrono::steady_clock::time_point begin_0 = chrono::steady_clock::now();
            choice = 0;
            //res0 = random_choice_add_node(g, current_solution);
            res = add_node_to_neighbours_accept(*g, current_solution);
            new_cost = current_cost + res.value_or(0);
            current_solution.book.b_add_node.last_add_operation = i;
            //chrono::steady_clock::time_point end_0 = chrono::steady_clock::now();
            //time_taken[0] += chrono::duration_cast<chrono::microseconds>(end_0 - begin_0).count();
        } else if (r == 1) {
            //chrono::steady_clock::time_point begin_3 = chrono::steady_clock::now();
            choice = 1;
            res = label_propagation_accept(*g, current_solution);
            new_cost = current_cost + res.value_or(0);
            //chrono::steady_clock::time_point end_3 = chrono::steady_clock::now();
            //time_taken[3] += chrono::duration_cast<chrono::microseconds>(end_3 - begin_3).count();
        } else if (r == 2) {
            //chrono::steady_clock::time_point begin_4 = chrono::steady_clock::now();
            choice = 2;
            res = remove_node_accept(*g, current_solution);
            new_cost = current_cost + res.value_or(0);
            //chrono::steady_clock::time_point end_4 = chrono::steady_clock::now();
            //time_taken[4] += chrono::duration_cast<chrono::microseconds>(end_4 - begin_4).count();
        } else if (r == 3) {
            //chrono::steady_clock::time_point begin_5 = chrono::steady_clock::now();
            choice = 3;
            res = add_node_to_set(*g, current_solution);
            new_cost = current_cost + res.value_or(0);
            //chrono::steady_clock::time_point end_5 = chrono::steady_clock::now();
            //time_taken[5] += chrono::duration_cast<chrono::microseconds>(end_5 - begin_5).count();
        } 
        else if (r == 4) {
            choice = 4;
            res = label_propagation_accept_weighted_random(*g, current_solution);
            //cout << "res has value: " << res.has_value() << "\n";
            new_cost = current_cost + res.value_or(0);
        } else if (r == 5) {
            choice = 5;
            res = label_propagation_accept_unchanged(*g, current_solution);
            new_cost = current_cost + res.value_or(0);
        } else if (r == 6) {
            choice = 6;
            res = add_node_to_neighbours_accept_unchanged(*g, current_solution);
            new_cost = current_cost + res.value_or(0);
        } else {
            choice = 7;
            //cout << "choice==7\n";
            res = sample_remove_add_3(*g, current_solution);
            new_cost = current_cost + res.value_or(0);
        } 

        if (i >= end_warmup) {
            find_prob_of_acceptance = 1000000 * exp(-(new_cost - current_cost)/t);
        } else {
            find_prob_of_acceptance = 800000;
        }
        
        if (new_cost > current_cost) {
            sum_delta += new_cost - current_cost;
            positive_delta_counter += 1;
        }
        if (new_cost <= current_cost || current_solution.ra.get_random_int() % 1000000 < find_prob_of_acceptance) {
            if (choice == 0 || choice == 6) {
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
            else if (choice == 1 || choice == 4 || choice == 5) {
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
            else if (choice == 7) {
                BRemoveAdd &b = current_solution.book.b_remove_add;
                if (res.has_value()) {
                    //cout << "time to remove add!\n";
                    for (pair<int, int> p : b.next_move_remove) {
                        current_solution.remove(p.first, p.second);
                    }
                    for (pair<int, int> p : b.next_move_add) {
                        current_solution.add(p.first, p.second);
                    }
                    current_cost = new_cost;
                }
            }
        }

        if (current_cost < best_cost) {
            //escape_counter = 0;
            best_cost = current_cost;
            last_iteration_of_best_solution = i;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
        }

        if (i % 1500 == 1499) { 
            cout << "Current cost: " << current_cost << "\n";
            //cout << "Current cost by cost function " << current_solution.cost_solution(g) << "\n";
            //current_solution.print_solution();
        }
        
        /**
        int actual_cost = current_solution.cost_solution(g);
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
        //current_solution.book.operation_number += 1;
        choices.push_back(choice);
        if (i == num_operations - 1) {
            solution_cost_iteration.push_back(current_cost);
        }
    }

    vector<int> count_choices(operations + 1, 0);
    for (int x : choices) count_choices[x] += 1;

    ShallowSolution shallow;
    if (with_cc) {
        shallow = from_cc_sol_to_sol(*g, best_solution, revert);
    } else {
        shallow = best_solution;
    }
    for (map<int, set<int>>::iterator it = shallow.clusters.begin(); it != shallow.clusters.end(); it++) {
        log_sol.clusters[it->first] = it->second;
    }
    for (map<int, set<int>>::iterator it = shallow.node_in_clusters.begin(); it != shallow.node_in_clusters.end(); it++) {
        log_sol.node_in_clusters[it->first] = it->second;
    }
    log_sol.operator_iteration = move(choices);
    //choices.clear();
    log_sol.solution_cost_iteration = move(solution_cost_iteration);
    //solution_cost_iteration.clear();
    log_sol.last_iteration_of_best_solution = last_iteration_of_best_solution;
    log_sol.time_taken = move(time_taken);
    log_sol.num_operations = num_operations;

    return log_sol;
}