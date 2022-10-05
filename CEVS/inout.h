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
#include "metaheuristics/alns2.h"
#include "metaheuristics/alns_tidy.h"
#include "metaheuristics/urh.h"
#include "utility/shallow_solution.h"
#include "bookkeep/b_merge.h"
#include <chrono>
#include "operators/add_node.h"
#include "solution_representation.h"
#include "bookkeep/bookkeep.h"
#include "utility/logging_solution.h"
#include <cmath>
#include <optional>
#include "utility/evaluate.h"
#include <ctype.h>


using namespace std;

void run_alns_on_validation_set();

void run_alns_on_heur_instances();

/**
 * Runs alns runs times on a single instance and prints the results in the results folder.
 * Each iteration uses num_operations operations. Graph is the graph of the instance (as read
 * by functions in read_file.cpp)
 */
void run_alns_on_single_instance(string &filename, Graph &g, int runs, int num_operations);

void run_on_karate_graph();

void run_on_football_graph();

void run_alns_on_gml(int num_iterations, int num_runs, string filename);

void run_alns_on_csv();

void test_p3();

void alns_scaling_p3();

void run_operation();

void test_evaluate();

void check_solution();

void run_alns_on_gz(int num_iterations, int num_runs, string filename);


#endif