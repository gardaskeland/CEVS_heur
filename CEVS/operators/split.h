#ifndef SPLIT_H
#define SPLIT_H

#include <map>
#include <queue>
#include <algorithm>
#include <set>
#include "../graph.h"
#include "../solution_representation.h"
#include "../utility/karger.h"
#include "../utility/weighted_random.h"
#include <chrono>
#include <optional>

using namespace std;

/**
 * @brief Computes the inner cost of a set in the solution representation, that is
 * (sum of cost of edge additions in set si) / |set si|.
 * 
 * @param g 
 * @param sol 
 * @param si 
 * @return int 
 */
int compute_inner_cost(Graph &g, SolutionRepresentation &sol, int si);

/**
 * @brief Picks a random set in the solution and finds a min cut in the set, giving
 * two disjoint new sets whose union is the original set. Returns the cost of the cut, and
 * stores the cut in sol.book.b_split.cut.
 * 
 * @param g 
 * @param sol 
 * @param si 
 * @return int 
 */
optional<int> random_choice_split(Graph &g, SolutionRepresentation &sol);

/**
* Stores info about a split that may be executed in sol.book.b_split and returns
* the cost of executing this split. It splits a set with as low inner cost as
* possible and uses Karger's algorithm to find an optimal way to split it (default).
* By setting f to clique_split, the clique split algorithm is used to find a cut
* instead.
*/
int greedy_split(Graph &g, SolutionRepresentation &sol, string f = "min_cut");

/**
 * @brief Executes the latest split.
 * 
 * @param sol 
 */
void do_split(SolutionRepresentation &sol);
#endif
