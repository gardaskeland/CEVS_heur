#include "plots.h"

void plot_weights(vector<LoggingSolution> &solutions, string &filename) {
    for (int i = 0; i < solutions.size(); i++) {
        vector<int> x;
        for (int j = 0; j < solutions[i].num_operations * solutions[i].change_weights_after; j = j + solutions[i].change_weights_after) {
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
        oss << "aaa"; //"results/weights_operators_for_iteration_" << i << "_of_" << filename << ".png";
        string s = oss.str();
        plt::title(s);
        plt::xlabel("operations");
        plt::ylabel("weight");
        plt::save(s);
    }
}

int main() {
    vector<LoggingSolution> sol;
    sol.emplace_back(LoggingSolution());
    sol[0].num_operations = 3;
    sol[0].change_weights_after = 1;
    sol[0].time_taken = vector<double>(1, 3);
    sol[0].weights_over_iteration.emplace_back(3, 1);
    sol[0].weights_over_iteration.emplace_back(3, 2);
    sol[0].weights_over_iteration.emplace_back(3, 3);
    string s = "aaa.txt";
    plot_weights(sol, s);
}