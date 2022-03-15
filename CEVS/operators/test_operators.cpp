#include "../solution_representation.h"
#include "../read_file.h"
#include "../graph.h"
#include "label_propagation.h"
#include "perturbation.h"

using namespace std;

int main() {
    string filename = "../../../../results/sol_to_other/FARZ10sol.csv";
    map<int, set<int>> from = read_solution(filename);

    filename = "../../../../FARZgraphs/FARZ10.gml";
    vector<vector<int>> adj = read_gml(filename);
    Graph g(adj);
    SolutionRepresentation sol(g.n, 10);
    for (auto it = from.begin(); it != from.end(); it++) {
        sol.add_set(it->second);
    }

    for (auto it = sol.clusters.begin(); it != sol.clusters.end(); it++) {
        cout << it-> first << ": ";
        for (int i : it-> second) {
            cout << i << " ";
        }
        cout << "\n";
    }
    //sol.calculate_node_in_clusters();
    
    int current_cost = sol.cost_solution(g);

    cout << "Before, cost is " << current_cost << "\n";
    sol.print_solution();

    int new_cost;

    for (int i = 0; i < 10; i++) {
        /**
        optional<int> res = label_propagation_accept(g, sol);
        new_cost = res.value_or(0);

        if (res.has_value() && new_cost < current_cost + 1) {
            tuple<int, int, int> move = sol.book.b_lp.next_move;
            sol.remove(get<0>(move), get<1>(move));
            if (get<2>(move) == -1) {
                set <int> new_set;
                new_set.insert(get<0>(move));
                sol.add_set(new_set);
            }
            else {
                sol.add(get<0>(move), get<2>(move));
            }
            current_cost = new_cost;
        }*/
        
        optional<int> res = add_set_over_uncovered(g, sol);
        new_cost = current_cost + res.value_or(0);

        if (res.has_value() && new_cost < current_cost + 5) {
            sol.add_set(sol.book.b_perturbation.set_to_add);
            current_cost = new_cost;
        }
        sol.print_solution();
        
        res = add_node_to_set(g, sol);
        new_cost = current_cost + res.value_or(0);

        if (res.has_value()) {
            sol.add(sol.book.b_add_node.v, sol.book.b_add_node.si);
            current_cost = new_cost;
        }

        int actual_cost = sol.cost_solution(g);
        if (current_cost != actual_cost) {
            cout << "ERROR in add_set_over_uncovered\n";
            cout << "current, actual: " << current_cost << " " << actual_cost << "\n";
            break;
        }

        sol.print_solution();
        cout << "\n--------\n";

        cout << "Current cost: " << current_cost << "\n";
    }

    current_cost += remove_nodes_(g, sol);

    cout << "After, cost is " << sol.cost_solution(g) << "\n";
    sol.print_solution();

}