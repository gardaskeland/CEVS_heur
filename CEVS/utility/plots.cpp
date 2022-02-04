#include "plots.h"

void plot_weights(vector<LoggingSolution> &solutions, string &filename) {
    for (int i = 0; i < solutions.size(); i++) {
        vector<int> x;
        for (int j = 0; j < solutions[i].num_operations * solutions[i].change_weights_after; solutions[i].change_weights_after++) {
            x.push_back(j);
        }
        vector<int> y;
        for (int j = 0; j < solutions[i].time_taken.size(); j++) {
            y.clear();
            for (vector<double> v : solutions[i].weights_over_iteration) {
                y.push_back(v[j]);
            }
            plt::plot(x, y);
        }
        ostringstream oss;
        oss.str(string());
        oss << "results/weights_operators_for_iteration_" << i << "_of_" << filename << ".png";
        string s = oss.str();
        plt::title(s);
        plt::xlabel("operations");
        plt::ylabel("weight");
        plt::save(s);
    }
}