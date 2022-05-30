#include "inout.h"

string integer_to_three_digits(int i) {
    ostringstream oss;
    if (i >= 100) {
        oss << i;
        return oss.str();
    }
    else if (i >= 10) {
        oss << 0 << i;
        return oss.str();
    } else {
        oss << 0 << 0 << i;
        return oss.str();
    }
}

vector<double> find_average_improvement_operations(vector<LoggingSolution> &solutions) {
    int n = solutions[0].time_taken.size();
    vector<double> results(n, 0);
    vector<int> operation_counter(n, 0);
    for (LoggingSolution sol : solutions) {
        for (int i = 0; i < sol.num_operations; i++) {
            results[sol.operator_iteration[i]] += sol.solution_cost_iteration[i+1] - sol.solution_cost_iteration[i];
            operation_counter[sol.operator_iteration[i]] += 1;   
        }
    }
    for (int i = 0; i < n; i++) {
        results[i] = results[i] / operation_counter[i];
    } 
    return results;
}

vector<double> find_average_time_operators(vector<LoggingSolution> &solutions) {
    int n = solutions[0].time_taken.size();
    vector<double> results(n, 0);
    vector<int> choices(n, 0);
    for (LoggingSolution sol : solutions) {
        for (int choice : sol.operator_iteration) {
            choices[choice] += 1;
        }
        for (int i = 0; i < n; i++) {
            results[i] += sol.time_taken[i];
        }
    }
    for (int i = 0; i < n; i++) {
        results[i] = results[i] / choices[i];
    }
    return results;
}

void write_weights_for_iterations(vector<LoggingSolution> &sol, string &filename) {
    ofstream out_file;
    out_file.open(filename);
    out_file << sol.size() << " "; //number of iterations on the input graph
    out_file << sol[0].num_operations << " ";
    out_file << sol[0].change_weights_after << " "; //After so many operations weights are changed
    //The number of different weights values over the run of the algorithm.
    out_file << sol[0].num_operations / sol[0].change_weights_after << "\n"; 
    for (LoggingSolution solution : sol) {
        for (vector<double> v : solution.weights_over_iteration) {
            for (double w : v) {
                out_file << w << " ";
            }
            out_file << "\n";
        }
    }
    out_file.close();
}

void write_cost_dev_for_iterations(vector<LoggingSolution> &sol, string &filename) {
    ofstream out_file;
    out_file.open(filename);
    out_file << sol.size() << " "; //number of iterations on the input graph
    out_file << sol[0].num_operations << "\n";
    for (LoggingSolution solution : sol) {
        for (int i = 0; i < solution.num_operations; i++) {
            out_file << solution.solution_cost_iteration[i] << " " << solution.operator_iteration[i] << "\n";
        }
        out_file << solution.solution_cost_iteration[solution.num_operations] << " -1\n";
    }
    out_file.close();
}

void run_alns_on_validation_set() {
    ostringstream oss;
    int num_runs = 5;
    int num_operations = 100000;
    ofstream out_file;
    out_file.open("results/alns_validation_results.txt");
    for (int i = 0; i < 20; i++) {
        string filename;
        oss.clear();
        oss.str(string());
        oss << "../../../../validation/data/FARZ_validation_" << i << ".gml";
        filename = oss.str();
        vector<vector<int>> adj = read_gml(filename);
        Graph g = Graph(adj);
        int best_cost = numeric_limits<int>::max();
        int sum_costs = 0;
        for (int j = 0; j < num_runs; j++) {
            LoggingSolution logsol;
            alns2_no_cc(g, logsol, num_operations);

            SolutionRepresentation calculate_sol = SolutionRepresentation(g.n, num_operations);
            map<int, set<int>> clusters = logsol.clusters;
            //cout << "a";
            for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
                //cout << "b";
                calculate_sol.add_set(it->second);
            }
            
            int cost = calculate_sol.cost_solution(g);
            if (cost < best_cost) {
                best_cost = cost;
            }
            sum_costs += cost;
        }
        cout << "For file " << i << ": best_cost: " << best_cost << ", average cost: " << 
        (double)sum_costs/(double)num_runs << "\n";
        out_file << best_cost << " " << (double)sum_costs/(double)num_runs << "\n";
    }
    out_file.close();
}

void run_alns_on_heur_instances() {
    int num_operations = 10000;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (int i = 1; i < 4; i = i + 2) {
        ostringstream oss;
        string filename;
        oss.clear();
        oss.str(string());
        oss << "../../../../../Master/heur/heur" << integer_to_three_digits(i) << ".gr";
        filename = oss.str(); // "../CEVStest/test_graphs/g8.txt";
        oss.clear();
        cout << "Working on file " << filename << "\n";
        vector<vector<int>> adj = read_gz_file(filename);
        Graph g = Graph(adj);

        int summed_costs = 0;
        int iterations = 5;
        int num_operators = 8;
        int best_cost = pow(2, 30);
        ShallowSolution best_solution;
        vector<LoggingSolution> solutions;
        vector<tuple<int, int, int>> op_solutions;
        vector<int> cost_of_solutions;
        vector<double> time_for_iterations;
        for (int j = 0; j < iterations; j++) {

            chrono::steady_clock::time_point begin_ = chrono::steady_clock::now();
            LoggingSolution sol;
            alns2_no_cc(g, sol, num_operations);
            chrono::steady_clock::time_point end_ = chrono::steady_clock::now();

            solutions.push_back(sol);
            cout << "Best solution:\n";
            double time_elapsed_ = chrono::duration_cast<chrono::microseconds>(end_ - begin_).count();
            time_for_iterations.push_back(time_elapsed_);
            cout << "time used in seconds for graph " << filename << ": " << time_elapsed_ / 1000000 << "\n";

            SolutionRepresentation calculate_sol = SolutionRepresentation(g.n, num_operations);
            map<int, set<int>> clusters = sol.clusters;
            //cout << "a";
            for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
                //cout << "b";
                calculate_sol.add_set(it->second);
            }
            calculate_sol.print_solution();
            
            tuple<int, int, int> cost_op = calculate_sol.cost_operations(g);
            int cost = get<0>(cost_op) + get<1>(cost_op) + get<2>(cost_op);
            op_solutions.push_back(cost_op);
            cost_of_solutions.push_back(cost);
            summed_costs += cost;
            if (cost < best_cost) {
                best_cost = cost;
                best_solution = sol;
            }
            cout << "Solution feasible: " << calculate_sol.simple_feasibility_check() << "\n";
            cout << "Cost of solution: " << cost << "\n";
            cout << "Number of splitting operations: " << calculate_sol.num_splits() << "\n";
            remove_nodes_(g, calculate_sol);
            cout << "Aft cer using remove nodes: " << "\n";
            calculate_sol.print_solution();
            cout << "Solution feasible: " << calculate_sol.simple_feasibility_check() << "\n";
            cout << "Cost of solution: " << calculate_sol.cost_solution(g) << "\n";
            cout << "Number of splitting operations: " << calculate_sol.num_splits() << "\n";
            cout << "\n-----------------------------------------------\n\n";
        }

        oss.str(string());
        oss << "results/weights-heur" << integer_to_three_digits(i) << ".txt";
        string p_file = oss.str();
        write_weights_for_iterations(solutions, p_file);
        oss.clear();
        oss.str(string());
        oss << "results/cost-heur" << integer_to_three_digits(i) << ".txt";
        p_file = oss.str();
        write_cost_dev_for_iterations(solutions, p_file);
        oss.clear();

        ofstream out_file;
        oss.str(string());
        oss << "results/heur" << integer_to_three_digits(i) << "all.txt";
        string out_all = oss.str();
        int sum_last_iteration = 0;
        vector<double> average_time_operators = find_average_time_operators(solutions);
        vector<double> average_improvement_operations = find_average_improvement_operations(solutions);
        double average_runtime = 0;
        for (int p = 0; p < iterations; p++) {
            sum_last_iteration += solutions[p].last_iteration_of_best_solution;
            average_runtime += time_for_iterations[p];
        }
        average_runtime = average_runtime / (double)iterations;
        out_file.open(out_all); 
        out_file << "instance: " << filename << "\n";
        out_file << "iterations: " << iterations << "\n";
        out_file << "operations per iteration: " << num_operations << "\n";
        out_file << "best solution:\n";
        out_file << best_solution.solution_as_string() << "\n";
        out_file << "cost of best solution: " << best_cost << "\n";
        out_file << "average cost of solutions: " << summed_costs / (double)iterations << "\n";
        out_file << "average last operation finding best solution: " << (double)sum_last_iteration / iterations << "\n";
        out_file << "average runtime: " << average_runtime / 1000000 << "\n";
        out_file << "------------------\n";
        
        out_file << "average time of add_nodes_to_neighbours: " << average_time_operators[0] / 1000000 << "\n";
        out_file << "average time of label_prop: " << average_time_operators[1] / 1000000 << "\n";
        out_file << "average time of remove_node: " << average_time_operators[2] / 1000000 << "\n";
        out_file << "average time of add_node_to_set " << average_time_operators[3] / 1000000 << "\n";
        out_file << "average time of label_prop_wr " << average_time_operators[4] / 1000000 << "\n";
        out_file << "average time of label_prop_unchanged " << average_time_operators[5] / 1000000 << "\n";
        out_file << "average time of add_nodes_to_neighbours_unchanged " << average_time_operators[6] / 1000000 << "\n";
        out_file << "average time of simple_remove_add_3 " << average_time_operators[7] / 1000000 << "\n";
        out_file << "------------------\n";

        out_file << "average improvement of add_all_nodes_to_neighbours " << average_improvement_operations[0] << "\n";
        out_file << "per second: " << average_improvement_operations[0] / (average_time_operators[0] / 1000000) << "\n";
        out_file << "average improvement of random_choice_split " << average_improvement_operations[1] << "\n";
        out_file << "per second: " << average_improvement_operations[1] / (average_time_operators[1] / 1000000) << "\n";
        out_file << "average improvement of weighted_random_merge " << average_improvement_operations[2] << "\n";
        out_file << "per second: " << average_improvement_operations[2] / (average_time_operators[2] / 1000000) << "\n";
        out_file << "average improvement of label_propagation_round " << average_improvement_operations[3] << "\n";
        out_file << "per second: " << average_improvement_operations[3] / (average_time_operators[3] / 1000000) << "\n";
        //out_file << "average improvement of remove_nodes_ " << average_improvement_operations[4] << "\n";
        //out_file << "per second: " << average_improvement_operations[4] / (average_time_operators[4] / 1000000) << "\n";
        //out_file << "average improvement of add_node_to_all " << average_improvement_operations[5] << "\n";
        //out_file << "per second: " << average_improvement_operations[5] / (average_time_operators[5] / 1000000) << "\n";

        out_file << "all solutions:\n";
        out_file << "------------------\n";
        for (int p = 0; p < iterations; p++) {
            out_file << "iteration " << p << ": " << solutions[p].solution_as_string() << "\n";
            out_file << "edge deletions: " << get<0>(op_solutions[p]) << "\n";
            out_file << "edge additions: " << get<1>(op_solutions[p]) << "\n";
            out_file << "vertex splittings: " << get<2>(op_solutions[p]) << "\n";
            out_file << "cost of solution: " << cost_of_solutions[p] << "\n";
            out_file << "time used on iteration: " << time_for_iterations[p] / 1000000 << "\n";
            out_file << "best solution found at iteration " << solutions[p].last_iteration_of_best_solution << "\n";
            out_file << "------------------\n";
        }
        out_file.close();
        cout << i << "\n";
    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    cout << "time used in seconds: " << time_elapsed / 1000000 << "\n";
}

/**
 * Removes the path from/a path to a file, returning only the filename
 */
string get_filename_after_path(string &filename) {
    string next, prev;
    stringstream str(filename);
    while(getline(str, next, '/')) {
        continue;
    }
    return next;
}

void run_alns_on_single_instance(string &filename, Graph &g, int runs, int num_operations) {
    ostringstream oss;
    cout << "Working on file " << filename << "\n";
    string filename_without_path = get_filename_after_path(filename);

    int summed_costs = 0;
    int num_operators = 5;
    int best_cost = pow(2, 30);
    ShallowSolution best_solution;
    vector<LoggingSolution> solutions;
    vector<tuple<int, int, int>> op_solutions;
    vector<int> cost_of_solutions;
    vector<double> time_for_iterations;
    for (int j = 0; j < runs; j++) {

        chrono::steady_clock::time_point begin_ = chrono::steady_clock::now();
        LoggingSolution sol;
        alns2(g, sol, num_operations);
        chrono::steady_clock::time_point end_ = chrono::steady_clock::now();

        solutions.push_back(sol);
        cout << "Best solution:\n";
        double time_elapsed_ = chrono::duration_cast<chrono::microseconds>(end_ - begin_).count();
        time_for_iterations.push_back(time_elapsed_);
        cout << "time used in seconds for graph " << filename << ": " << time_elapsed_ / 1000000 << "\n";

        SolutionRepresentation calculate_sol = SolutionRepresentation(g.n, num_operations);
        map<int, set<int>> clusters = sol.clusters;
        //cout << "a";
        for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
            //cout << "b";
            calculate_sol.add_set(it->second);
        }
        calculate_sol.print_solution();
        
        tuple<int, int, int> cost_op = calculate_sol.cost_operations(g);
        int cost = get<0>(cost_op) + get<1>(cost_op) + get<2>(cost_op);
        op_solutions.push_back(cost_op);
        cost_of_solutions.push_back(cost);
        summed_costs += cost;
        if (cost < best_cost) {
            best_cost = cost;
            best_solution = sol;
        }
        cout << "Solution feasible: " << calculate_sol.simple_feasibility_check() << "\n";
        cout << "Cost of solution: " << cost << "\n";
        cout << "Number of splitting operations: " << calculate_sol.num_splits() << "\n";
        remove_nodes_(g, calculate_sol);
        cout << "Aft cer using remove nodes: " << "\n";
        calculate_sol.print_solution();
        cout << "Solution feasible: " << calculate_sol.simple_feasibility_check() << "\n";
        cout << "Cost of solution: " << calculate_sol.cost_solution(g) << "\n";
        cout << "Number of splitting operations: " << calculate_sol.num_splits() << "\n";
        cout << "\n-----------------------------------------------\n\n";
    }

    oss.str(string());
    oss << "results/weights-" << filename_without_path << ".txt";
    string p_file = oss.str();
    write_weights_for_iterations(solutions, p_file);
    oss.clear();
    oss.str(string());
    oss << "results/cost-" << filename_without_path << ".txt";
    p_file = oss.str();
    write_cost_dev_for_iterations(solutions, p_file);
    oss.clear();

    ofstream out_file;
    oss.str(string());
    oss << "results/all-" << filename_without_path << ".txt";
    string out_all = oss.str();
    int sum_last_iteration = 0;
    vector<double> average_time_operators = find_average_time_operators(solutions);
    vector<double> average_improvement_operations = find_average_improvement_operations(solutions);
    double average_runtime = 0;
    for (int p = 0; p < runs; p++) {
        sum_last_iteration += solutions[p].last_iteration_of_best_solution;
        average_runtime += time_for_iterations[p];
    }
    average_runtime = average_runtime / (double)runs;
    out_file.open(out_all); 
    out_file << "instance: " << filename << "\n";
    out_file << "nodes: " << g.adj.size() << "\n";
    out_file << "edges: " << g.num_edges << "\n";
    out_file << "runs: " << runs << "\n";
    out_file << "operations per iteration: " << num_operations << "\n";
    out_file << "best solution:\n";
    out_file << best_solution.solution_as_string() << "\n";
    out_file << "cost of best solution: " << best_cost << "\n";
    out_file << "average cost of solutions: " << summed_costs / (double)runs << "\n";
    out_file << "average last operation finding best solution: " << (double)sum_last_iteration / runs << "\n";
    out_file << "average runtime: " << average_runtime / 1000000 << "\n";
    out_file << "------------------\n";
    
    out_file << "average time of add_all_nodes_to_neighbours: " << average_time_operators[0] / 1000000 << "\n";
    out_file << "average time of random_choice_split: " << average_time_operators[1] / 1000000 << "\n";
    out_file << "average time of weighted_random_merge: " << average_time_operators[2] / 1000000 << "\n";
    out_file << "average time of label_propagation_round " << average_time_operators[3] / 1000000 << "\n";
    out_file << "average time of remove_nodes_ " << average_time_operators[4] / 1000000 << "\n";
    out_file << "average time of add_node_to_all " << average_time_operators[5] / 1000000 << "\n";
    out_file << "------------------\n";

    out_file << "average improvement of add_all_nodes_to_neighbours " << average_improvement_operations[0] << "\n";
    out_file << "per second: " << average_improvement_operations[0] / (average_time_operators[0] / 1000000) << "\n";
    out_file << "average improvement of random_choice_split " << average_improvement_operations[1] << "\n";
    out_file << "per second: " << average_improvement_operations[1] / (average_time_operators[1] / 1000000) << "\n";
    out_file << "average improvement of weighted_random_merge " << average_improvement_operations[2] << "\n";
    out_file << "per second: " << average_improvement_operations[2] / (average_time_operators[2] / 1000000) << "\n";
    out_file << "average improvement of label_propagation_round " << average_improvement_operations[3] << "\n";
    out_file << "per second: " << average_improvement_operations[3] / (average_time_operators[3] / 1000000) << "\n";
    out_file << "average improvement of remove_nodes_ " << average_improvement_operations[4] << "\n";
    out_file << "per second: " << average_improvement_operations[4] / (average_time_operators[4] / 1000000) << "\n";
    out_file << "average improvement of add_node_to_all " << average_improvement_operations[5] << "\n";
    out_file << "per second: " << average_improvement_operations[5] / (average_time_operators[5] / 1000000) << "\n";

    out_file << "all solutions:\n";
    out_file << "------------------\n";
    for (int p = 0; p < runs; p++) {
        out_file << "iteration " << p << ": " << solutions[p].solution_as_string() << "\n";
        out_file << "edge deletions: " << get<0>(op_solutions[p]) << "\n";
        out_file << "edge additions: " << get<1>(op_solutions[p]) << "\n";
        out_file << "vertex splittings: " << get<2>(op_solutions[p]) << "\n";
        out_file << "cost of solution: " << cost_of_solutions[p] << "\n";
        out_file << "time used on iteration: " << time_for_iterations[p] / 1000000 << "\n";
        out_file << "best solution found at iteration " << solutions[p].last_iteration_of_best_solution << "\n";
        out_file << "------------------\n";
    }
    out_file.close();
}

void run_on_karate_graph() {
    cout << "adjacency: \n";
    string filename = "../../../karate78.csv/edges.csv";
    vector<vector<int>> adj = read_csv_graph(filename, 37);
    for (vector<int> vec : adj) {
        for (int i : vec) {
            cout << i << " ";
        }
        cout << "\n";
    }
    ofstream original_clusters_karate;
    original_clusters_karate.open("utility/Overlapping-NMI-master/original_clusters_karate.txt");
    cout << "Ground thruth: \n";
    filename = "../../../karate78.csv/nodes.csv";
    map<int, set<int>> ground_truth = read_csv_groups_karate(filename);
    for (auto it = ground_truth.begin(); it != ground_truth.end(); it++) {
        for (int i : it->second) {
            cout << i << " ";
            original_clusters_karate << i << " ";
        }
        cout << "\n";
        original_clusters_karate << "\n";
    }
    original_clusters_karate.close();

    for (int i = 0; i < 5; i++) {

    Graph g(adj);
    LoggingSolution sol;
    int operations = 3000;

    alns(g, sol, operations);

    ofstream alns_solution_karate;
    alns_solution_karate.open("utility/Overlapping-NMI-master/alns_solution_karate.txt");
    cout << "Solution given by alns: \n";
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        for (int i : it-> second) {
            cout << i << " ";
            alns_solution_karate << i << " ";
        }
        cout << "\n";
        alns_solution_karate << "\n";
    }
    alns_solution_karate.close();

    SolutionRepresentation calculate_sol(g.n, operations);
    map<int, set<int>> clusters = sol.clusters;
            //cout << "a";
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
                //cout << "b";
        calculate_sol.add_set(it->second);
    }

    tuple<int, int, int> cost_operations = calculate_sol.cost_operations(g);
    cout << "deletions, additions, splits: " << get<0>(cost_operations) << " " << get<1>(cost_operations) << \
        " " << get<2>(cost_operations) << "\n";
    cout << "cost of solution: " << get<0>(cost_operations) + get<1>(cost_operations) + get<2>(cost_operations) << "\n";

    pair<double, double> p = find_majority_accuracy(g, calculate_sol, ground_truth);
    cout << "Majority accuracy: " << p.first << "\n";
    cout << "Majority inaccuracy: " << p.second << "\n";
    cout << "\n------------------\n";

    }


    //Find majority accuracy: For each set in the solution, if it is a subset of a ground truth set, we say that
    //all vertices are classified correctly. If it is not, we see how many vertices are in the majority set and
    //say that they have been classified correctly. In the end we sum up the number of vertices that are
    //classified correctly.
}

void run_on_football_graph() {
    string filename = "../../../football.csv/edges.csv";
    vector<vector<int>> adj = read_csv_graph(filename, 115);
    for (vector<int> vec : adj) {
        for (int i : vec) {
            cout << i << " ";
        }
        cout << "\n";
    }
    ofstream original_clusters_football;
    original_clusters_football.open("utility/Overlapping-NMI-master/original_clusters_football.txt");
    cout << "Ground thruth: \n";
    filename = "../../../football.csv/nodes.csv";
    map<int, set<int>> ground_truth = read_csv_groups_football(filename);
    for (auto it = ground_truth.begin(); it != ground_truth.end(); it++) {
        for (int i : it->second) {
            cout << i << " ";
            original_clusters_football << i << " ";
        }
        cout << "\n";
        original_clusters_football << "\n";
    }
    original_clusters_football.close();

    for (int i = 0; i < 5; i++) {

    Graph g(adj);
    LoggingSolution sol;
    int operations = 3000;

    alns(g, sol, operations);

    ofstream alns_solution;
    alns_solution.open("utility/Overlapping-NMI-master/alns_solution_football.txt");
    cout << "Solution given by alns: \n";
    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        for (int i : it-> second) {
            cout << i << " ";
            alns_solution << i << " ";
        }
        cout << "\n";
        alns_solution << "\n";
    }
    alns_solution.close();

    SolutionRepresentation calculate_sol(g.n, operations);
    map<int, set<int>> clusters = sol.clusters;
            //cout << "a";
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
                //cout << "b";
        calculate_sol.add_set(it->second);
    }

    tuple<int, int, int> cost_operations = calculate_sol.cost_operations(g);
    cout << "deletions, additions, splits: " << get<0>(cost_operations) << " " << get<1>(cost_operations) << \
        " " << get<2>(cost_operations) << "\n";
    cout << "cost of solution: " << get<0>(cost_operations) + get<1>(cost_operations) + get<2>(cost_operations) << "\n";

    pair<double, double> p = find_majority_accuracy(g, calculate_sol, ground_truth);
    cout << "Majority accuracy: " << p.first << "\n";
    cout << "Majority inaccuracy: " << p.second << "\n";
    cout << "\n------------------\n";

    }
}

SolutionRepresentation calculate_and_print_sol(LoggingSolution &sol, Graph &g, int operations) {
    SolutionRepresentation calculate_sol(g.n, operations);
    map<int, set<int>> clusters = sol.clusters;
            //cout << "a";
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
                //cout << "b";
        calculate_sol.add_set(it->second);
    }

    tuple<int, int, int> cost_operations = calculate_sol.cost_operations(g);
    cout << "deletions, additions, splits: " << get<0>(cost_operations) << " " << get<1>(cost_operations) << \
        " " << get<2>(cost_operations) << "\n";
    cout << "cost of solution: " << get<0>(cost_operations) + get<1>(cost_operations) + get<2>(cost_operations) << "\n";
    calculate_sol.print_solution();

    return calculate_sol;
}


struct cmp_second {
    bool operator() (pair<int, int> &left, pair<int, int> &right) {
        return left.second < right.second;
    }
};

//Difficulty of problem depends on number of p3s?
void test_p3() {
    ostringstream str;
    vector<pair<int, int>> score;
    ofstream out_file;
    string out_file_name = "results/graphs_in_test_sorted_by_num_p3.txt";
    out_file.open(out_file_name);
    for (int i = 0; i < 200; i++) {
        str.clear();
        str.str(string());
        str << "../../../../../Master/train/data/FARZ_train_" << i << ".gml";
        string filename = str.str();
        vector<vector<int>> adj = read_gml(filename);
        Graph g(adj);
        g.find_all_p3s();
        //for (tuple<int, int, int> t : g.all_p3s) {
            //cout << get<0>(t) + 1 << " " << get<1>(t) + 1 << " " << get<2>(t) + 1 << "\n";
        //}
        cout << "graph " << i << ": num p3's = " << g.all_p3s.size() << "\n";
        score.push_back(make_pair(i, g.all_p3s.size()));
    }
    sort(score.begin(), score.end(), cmp_second());
    cout << "graphs sorted by p3s: \n";
    for (auto p : score) {
        cout << p.first << " " << p.second << "\n";
        if (p.second < 60000) out_file << p.first << "\n";
    }
    out_file.close();

}

void run_alns_on_gml() {
    ostringstream str;
    //vector<int> v = {2, 6, 10, 14, 18, 22, 26, 30};
    for (int i = 0; i < 10; i++) {
        str.clear();
        str.str(string());
        str << "../../../data/test_data/FARZ_100_" << i << ".gml";
        string filename = str.str();
        vector<vector<int>> adj = read_gml(filename);

        Graph g(adj);
        run_alns_on_single_instance(filename, g, 5, 80000);
    }
}

void alns_scaling_p3() {
    stringstream str;
    ostringstream oss;
    vector<pair<int, int>> score;
    int num_operations = 10000;
    int run_times = 5;
    ofstream out_file;
    oss.str(string());
    oss << "results/avg_minus_best_p3_scale.txt";
    string out_all = oss.str();
    out_file.open(out_all);
    fstream in_file;
    string instring;
    in_file.open("../../../../../Master/train/data_for_p3_scaling_experiments/graphs_in_test_sorted_by_num_p3.txt", ios::in);
    vector<int> graph_numbers;
    while (getline(in_file, instring)) {
        graph_numbers.push_back(stoi(instring));
    } //Sorted by number of p3's
    //sort(graph_numbers.begin(), graph_numbers.end());
    chrono::steady_clock::time_point begin, end;
    for (int i : graph_numbers) {
        str.clear();
        str.str(string());
        str << "../../../../../Master/train/data_for_p3_scaling_experiments/FARZ_train_" << i << ".gml";
        string filename = str.str();
        vector<vector<int>> adj = read_gml(filename);
        Graph g(adj);
        g.find_all_p3s();
        int p3_num = g.all_p3s.size();

        int best_cost = (1 << 15);
        int sum_cost = 0;
        double total_time = 0.0;
        
        for(int i = 0; i < run_times; i++) {
            LoggingSolution logsol;
            begin = chrono::steady_clock::now();
            alns2_no_cc(g, logsol, num_operations);
            end = chrono::steady_clock::now();
            total_time += chrono::duration_cast<chrono::microseconds>(end - begin).count();
            SolutionRepresentation calc(g.n, 100);
            calc.clusters = logsol.clusters;
            calc.node_in_clusters = logsol.node_in_clusters;
            int cost = calc.cost_solution(g);
            sum_cost += cost;
            if (best_cost > cost) {
                best_cost = cost;
            }
        }

        out_file << i << " " << g.n << " " << g.num_edges << " " << p3_num << " " << (sum_cost / run_times) - best_cost << \
            " " << total_time / (run_times*1000000) <<  "\n";
    }
    out_file.close();
}

void run_alns_on_csv() {
    ostringstream str;
    vector<string> v = {"celegans_metabolic", "cs_department", "facebook_friends", "football.csv", \
                        "game_thrones", "jazz_collab", "karate78.csv", "law_firm", "revolution", "unicodelang"};
    vector<int> num_nodes = {453, 61, 362, 115, 107, 198, 34, 71, 141, 868};
    for (int i = 0; i < 10; i++) {
        str.clear();
        str.str(string());
        str << "../../../../../Master/Testsets/" << v[i] << "/edges.csv";
        string filename = str.str();
        vector<vector<int>> adj = read_csv_graph(filename, num_nodes[i]);

        Graph g(adj);
        run_alns_on_single_instance(filename, g, 5, 80000);
    }
}

void run_operation() {
    string filename = "../../../FARZgraphs/FARZ_500_edges_100.gml";
    vector<vector<int>> adj = read_gml(filename);
    Graph g(adj);
    LoggingSolution log_sol;
    int num_operations = 5000;
    alns2(g, log_sol, num_operations);

    SolutionRepresentation calculate_sol(g.n, num_operations);
    map<int, set<int>> clusters = log_sol.clusters;
            //cout << "a";
    for (map<int, set<int>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
                //cout << "b";
        calculate_sol.add_set(it->second);
    }
    cout << "Final cost: " << calculate_sol.cost_solution(g) << "\n";
    cout << "Solution: \n";
    calculate_sol.print_solution();
}

void test_evaluate() {
    string filename = "../../../../../Master/test/data/FARZ_test_0.gml";
    vector<vector<int>> adj = read_gml(filename);
    Graph g(adj);
    SolutionRepresentation sol(g.n, 1000);
    sol.initial_solution(g.n);
    evaluate_alns(g, sol, filename);

}