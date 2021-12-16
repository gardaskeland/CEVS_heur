#ifndef KARGER_H
#define KARGER_H

#include <algorithm>
#include <set>
#include "../graph.h"
#include "../solution_representation.h"
#include <vector>
#include <math.h>
#include <map>

using namespace std;

/**
 * @brief Finds a minimum cut of set si in the solution with high probability (1/e for fail).
 * Note that set si in sol must be a connected component.
 * 
 * @param g 
 * @param sol 
 * @param si 
 * @return pair<set<int>, set<int>> 
 */
pair<set<int>, set<int>> find_min_cut(Graph &g, SolutionRepresentation &sol, int &si);

#endif