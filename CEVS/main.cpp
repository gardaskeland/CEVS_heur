
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


//This works now, but very slowly already for 120 vertices.
int main() {
    //debug with valgrind?
    int num_operations = 500;
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
        static Graph g = Graph(adj);

        int summed_costs = 0;
        int iterations = 3;
        int best_cost = pow(2, 30);
        ShallowSolution best_solution;
        vector<LoggingSolution> solutions;
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

            int cost = calculate_sol.cost_solution(g);
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
        fstream out_file;
        oss.str(string());
        oss << "results/heur" << integer_to_three_digits(i) << "all.txt";
        string out_all = oss.str();  
        out_file.open(out_all); 
        out_file << "instance: " << filename << "\n";
        out_file << "iterations: " << iterations << "\n";
        out_file << "best solution:\n";
        out_file << best_solution.solution_as_string() << "\n";
        out_file << "cost of best solution: " << best_cost << "\n";
        out_file << "average cost of solutions: " << summed_costs / (double)iterations << "\n";
        out_file << "all solutions:\n";
        out_file << "------------------\n";
        for (int i = 0; i < iterations; i++) {
            out_file << "iteration " << i << ": " << solutions[i].solution_as_string() << "\n";
            out_file << "cost of solution: " << cost_of_solutions[i] << "\n";
            out_file << "time used on iteration: " << time_for_iterations[i] / 1000000 << "\n";
            out_file << "------------------\n";
        }
        out_file.close();

        




    }
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    double time_elapsed = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    cout << "time used in seconds: " << time_elapsed / 1000000 << "\n";

}