#ifndef INOUT_H
#define INOUT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "read_file.h"
#include "graph.h"
//#include "metaheuristics/local_search.h"
#include "metaheuristics/simulated_annealing.h"
#include "metaheuristics/local_search_lp.h"
#include "metaheuristics/alns.h"
#include "utility/shallow_solution.h"
#include "bookkeep/b_merge.h"
#include <chrono>
#include "operators/add_node.h"
#include "solution_representation.h"
#include "bookkeep/bookkeep.h"
#include "utility/logging_solution.h"
#include <cmath>
#include <optional>

using namespace std;

void run_alns_on_heur_instances();

void run_on_karate_graph();


#endif