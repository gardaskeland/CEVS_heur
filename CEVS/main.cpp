
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "read_file.h"
#include "graph.h"
//#include "metaheuristics/local_search.h"
#include "metaheuristics/simulated_annealing.h"
#include "metaheuristics/local_search_lp.h"
#include "metaheuristics/alns.h"
#include "utility/shallow_solution.h"
#include "bookkeep/b_merge.h"
#include <chrono>
#include "operators/add_node.h"
#include "solution_representation.h"
#include "bookkeep/bookkeep.h"
#include "utility/logging_solution.h"
#include <cmath>
#include "utility/plots.h"
using namespace std;

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
/**
vector<double> print_weights(LoggingSolution &sol, int iteration, string &filename) {
    
}
*/


//This works now, but very slowly already for 120 vertices.
int main() {
    int num_operations = 2000;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    for (int i = 3; i < 4; i = i + 2) {
        ostringstream oss;
        string filename;
        oss.clear();
        oss.str(string());
        oss << "../../../heur/heur" << integer_to_three_digits(i) << ".gr";
        filename = oss.str(); // "../CEVStest/test_graphs/g8.txt";
        oss.clear();
        cout << "Working on file " << filename << "\n";
        vector<vector<int>> adj = read_gz_file(filename);
        Graph g = Graph(adj);

        int summed_costs = 0;
        int iterations = 2;
        int num_operators = 6;
        int best_cost = pow(2, 30);
        ShallowSolution best_solution;
        vector<LoggingSolution> solutions;
        vector<tuple<int, int, int>> op_solutions;
        vector<int> cost_of_solutions;
        vector<double> time_for_iterations;
        for (int j = 0; j < iterations; j++) {

            chrono::steady_clock::time_point begin_ = chrono::steady_clock::now();
            LoggingSolution sol;
            alns(g, sol, num_operations);
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
            cout << "After using remove nodes: " << "\n";
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
        for (int p = 0; p < iterations; p++) {
            sum_last_iteration += solutions[p].last_iteration_of_best_solution;
        } 
        out_file.open(out_all); 
        out_file << "instance: " << filename << "\n";
        out_file << "iterations: " << iterations << "\n";
        out_file << "operations per iteration: " << num_operations << "\n";
        out_file << "best solution:\n";
        out_file << best_solution.solution_as_string() << "\n";
        out_file << "cost of best solution: " << best_cost << "\n";
        out_file << "average cost of solutions: " << summed_costs / (double)iterations << "\n";
        out_file << "average last operation finding best solution: " << (double)sum_last_iteration / iterations << "\n";
        out_file << "------------------\n";
        
        out_file << "average time of random_choice_add_node: " << average_time_operators[0] / 1000000 << "\n";
        out_file << "average time of random_choice_split: " << average_time_operators[1] / 1000000 << "\n";
        out_file << "average time of weighted_random_merge: " << average_time_operators[2] / 1000000 << "\n";
        out_file << "average time of label_propagation_round " << average_time_operators[3] / 1000000 << "\n";
        out_file << "average time of remove_nodes_ " << average_time_operators[4] / 1000000 << "\n";
        out_file << "average time of add_node_to_all " << average_time_operators[5] / 100000 << "\n";
        out_file << "------------------\n";

        out_file << "average improvement of random_choice_add_node " << average_improvement_operations[0] << "\n";
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
        for (int p = 0; p < iterations; p++) {
            out_file << "iteration " << p << ": " << solutions[p].solution_as_string() << "\n";
            out_file << "edge deletions: " << get<0>(op_solutions[p]) << "\n";
            out_file << "edge additions: " << get<1>(op_solutions[p]) << "\n";
            out_file << "vertex splittings: " << get<2>(op_solutions[p]) << "\n";
            out_file << "cost of solution: " << cost_of_solutions[p] << "\n";
            out_file << "time used on iteration: " << time_for_iterations[p] / 1000000 << "\n";
            out_file << "------------------\n";
        }
        out_file.close();
        cout << i << "\n";

        




    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    cout << "time used in seconds: " << time_elapsed / 1000000 << "\n";

}