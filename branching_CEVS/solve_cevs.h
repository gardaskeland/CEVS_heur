#ifndef SOLVE_CEVS_H
#define SOLVE_CEVS_H

#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <optional>
#include <tuple>
#include <set>
#include <queue>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <ratio>
#include <chrono>

#define tri tuple<int, int, int>

using namespace std;

struct info_struct {
    //Number of vertices in input graph
    int n;
    //Maximal number of allowed operations
    int k;
    //Number of vertices in the current graph
    int n_;
    //Number of operations executed. AKA depth of recursion. When k, cannot recurse further
    int k_;

    vector<int> progress;

    chrono::steady_clock::time_point last_checkpoint;

    chrono::steady_clock::time_point iteration_start;
    //true when we have spent more time on one choice of p3 than we would like to
    bool end_iteration;
};

bool recursion(vector<list<int>> &adj, info_struct &info);

#endif
