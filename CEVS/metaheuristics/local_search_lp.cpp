#include "local_search_lp.h"

ShallowSolution local_search_lp(Graph &g, int &num_operations) {
    RevertKernel revert;
    WeightedGraph wg = find_critical_clique_graph(g, revert);
    SolutionRepresentation current_solution = SolutionRepresentation(wg.n, num_operations);
    current_solution.initial_solution(wg.n);
    ShallowSolution best_solution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
    int current_cost = current_solution.cost_solution(wg);
    cout << "cost of initial solution: " << current_cost << "\n";
    Bookkeep book = Bookkeep(num_operations);
    int best_cost = current_cost;
    cout << "Starting local_search_lp\n";
    int converge_counter = 0;

    for (int i = 0; i < num_operations; i++) {
        //Seems like a bad idea.
        /**
        for (int j = 0; j < 1; j++) {
            if (converge_counter < 2) break;
            current_cost += weighted_random_merge(g, current_solution);
            current_solution.merge(current_solution.book.b_merge.si, current_solution.book.b_merge.sj);
            current_solution.book.b_merge.last_merge_operation = i;
            int may_add = random_choice_split(g, current_solution);
            set<int> indices = current_solution.get_set_indices_as_set();
            if (!(current_solution.book.b_split.si == -1) &&
                indices.find(current_solution.book.b_split.si) != indices.end()) {
                pair<set<int>, set<int>> p = current_solution.book.b_split.cut;
                current_solution.disjoint_split(current_solution.book.b_split.si, p.first, p.second);
                current_cost += may_add;
           }
        }
        */
        current_cost += add_node_to_all(wg, current_solution);
        for (int j = 0; j < 10; j++) {
            /*
            current_cost += random_choice_add_node(g, current_solution, book);
            set<int> indices = current_solution.get_set_indices_as_set();
            if (!(current_solution.book.b_add_node.v == -1) &&
                indices.find(current_solution.book.b_add_node.si) != indices.end()) {
                current_solution.add(current_solution.book.b_add_node.v, current_solution.book.b_add_node.si);
            }*/
            
            //cout << "current_cost: " << current_cost << "\n";
            //cout << "actual_cost: " << current_solution.cost_solution(g) << "\n";
            current_cost += vertex_isolator(wg, current_solution) + vertex_mover(wg, current_solution) + \
            add_adjacent_vertex(wg, current_solution); //clique_splitter(wg, current_solution);
        }
        
        for (double j = 0; j < 10; j++) { //max((double)15, 15*converge_counter*0.3); j++) {
            current_cost += label_propagation_round(wg, current_solution);
            //cout << "current_cost: " << current_cost << "\n";
            //cout << "actual_cost: " << current_solution.cost_solution(g) << "\n";
        }
        current_cost += remove_nodes(wg, current_solution);

        //Also using some merge and split with hope of speedup.
        
        if (current_cost < best_cost) {
            best_cost = current_cost;
            best_solution = ShallowSolution(current_solution.get_clusters(), current_solution.get_node_in_clusters());
            converge_counter = 1;
        } else {
            converge_counter += 1;
            if (converge_counter == 30) {
                cout << "Solution has converged after " << i << " rounds\n";
                break;
            }
        }
        //cout << "Average set size: " << current_solution.avg_set_size() << "\n";
        //cout << "current_cost: " << current_cost << "\n";
        current_solution.book.operation_number += 1;
        //cout << "actual_cost: " << current_solution.cost_solution(wg) << "\n";
        //current_solution.print_solution();
    }
    return from_cc_sol_to_sol(g, best_solution, revert);
}