#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include "graph.h"
#include <optional>
#include <queue>
#include "binary_search.h"

using namespace std;

int cost_adding_edge(Graph &g, int u, int v);

int cost_removing_edge(Graph &g, int u, int v);

void make_permanent_or_forbidden(double pth, double fth, Graph &g, int u, int v);

optional<tuple<int, int, int>> find_conflict_triple(Graph &g);

//Returns the total number of operations executed
int greedy_heuristic(Graph &g);

#endif