#ifndef LOGGING_SOLUTION_H
#define LOGGING_SOLUTION_H

#include "shallow_solution.h"
#include <vector>

using namespace std;

class LoggingSolution : public ShallowSolution {
    public:
        vector<int> solution_cost_iteration;
        //num of operator executed at iteration
        vector<int> operator_iteration;
        //cost change over the iteration
        //vector<int> cost_change_iteration;
        int last_iteration_of_best_solution;
        vector<vector<double>> weights_over_iteration;
        int change_weights_after;
        vector<double> time_taken;
        //total number of operations used to find the solution
        int num_operations;


    LoggingSolution(ShallowSolution &sol) {
        clusters = sol.clusters;
        node_in_clusters = sol.node_in_clusters;
    }

    LoggingSolution() {

    };
};

#endif