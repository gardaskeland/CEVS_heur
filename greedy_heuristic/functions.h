#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include "graph_.h"
#include <optional>
#include <queue>
#include "binary_search.h"
#include <algorithm>
#include <random>
#include <set>
#include "../CEVS/graph.h"
#include "../CEVS/solution_representation.h"

using namespace std;

int cost_adding_edge(Graph_ &g, int u, int v);

int cost_removing_edge(Graph_ &g, int u, int v);

int make_permanent_or_forbidden(double pth, double fth, Graph_ &g, int u, int v);

optional<tuple<int, int, int>> find_conflict_triple(Graph_ &g);

//Returns the total number of operations executed
vector<set<int>> greedy_heuristic(Graph_ &g);

#endif