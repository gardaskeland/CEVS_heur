#ifndef PLOTS_H
#define PLOTS_H

#include "matplotlibcpp.h"
#include "logging_solution.h"
#include <vector>

using namespace std;
namespace plt = matplotlibcpp;

/**
 * Plots the weights and how they change over the algorithm.
 */
void plot_weights(vector<LoggingSolution> &solutions);

#endif