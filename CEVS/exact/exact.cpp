#include "../solution_representation.h"
#include "../graph.h"
#include "../read_file.h"
#include <cmath>

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

void recursion(Graph &g, vector<int> &set_stack, int &best_cost_p, SolutionRepresentation &best_solution_p, int p, int last) {
    if (p == 0) {
        int check_feasibility = 0;
        for (int i : set_stack) {
            check_feasibility = check_feasibility | i;
        }
        if (check_feasibility != pow(2, g.n) - 1) return;
        SolutionRepresentation sol = SolutionRepresentation(g.n, 1);
        set<int> si;
        for (int i : set_stack) {
            si.clear();
            for (int j = 0; j < g.n; j++) {
                if ((i & (1 << j) ^ 0) != 0) {
                    si.insert(j);
                } else {
                }
            }
            for (int x : si) {
            }
            sol.add_set(si);
        }
        int cost = sol.cost_solution(g);
        if (cost < best_cost_p) {
            best_cost_p = cost;
            best_solution_p = sol;
        }
        return;
    }
    for (int i = last + 1; i < pow(2, g.n); i++) {
        set_stack.push_back(i);
        recursion(g, set_stack, best_cost_p, best_solution_p, p-1, i);
        set_stack.pop_back();
    }
    
}

int main() {
    ostringstream oss;
    string filename;
    oss.clear();
    oss.str(string());
    oss << "gexc3.txt"; //"../../../../heur/heur" << integer_to_three_digits(1) << ".gr";
    filename = oss.str(); // "../CEVStest/test_graphs/g8.txt";
    oss.clear();
    cout << "Working on file " << filename << "\n";
    vector<vector<int>> adj = read_gz_file(filename);
    Graph g = Graph(adj);
    SolutionRepresentation sol; //g.n, 1
    vector<int> best_cost;
    int best_cost_p;
    SolutionRepresentation best_solution_p;
    SolutionRepresentation best_solution;
    sol.initial_solution_complete_graph(g.n);
    int cost_complete_graph = sol.cost_solution(g);
    int best_cost_overall = cost_complete_graph;
    best_cost.push_back(best_cost_overall);
    vector<int> set_stack;
    cout << "Cost of complete graph: " << cost_complete_graph << "\n\n----------\n";
    //p is number of sets in solution.
    for (int p = 2; p < g.n; p++) {
        set_stack.empty();
        best_solution_p = SolutionRepresentation(g.n, 1);
        best_cost_p = 10000000;
        cout << "start recursion for " << p << "\n";
        recursion(g, set_stack, best_cost_p, best_solution_p, p, 0);
        cout << "recursion done for " << p << "\n";
        if (best_cost_p <= best_cost_overall) {
            best_cost_overall = best_cost_p;
            best_solution = best_solution_p;
        } else {
            
        }
        cout << "Best solution with size " << p << ":\n";
        best_solution_p.print_solution();
        cout << "Cost of solution: " << best_cost_p << "\n";
        cout <<"\n------\n";
    }
    cout << "Cost of best solution: " << best_cost_overall << "\n";
    cout << "Best solution: \n";
    best_solution.print_solution();
    cout << "Sets: " << best_solution.num_sets() << "\n";

}
