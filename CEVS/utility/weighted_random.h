#ifndef WEIGHTED_RANDOM_H
#define WEIGHTED_RANDOM_H

#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

/**
 * Return a weighted random index so that there is 1/log_base chance that the index is and
 * 1/(log_base)^k that the index is k-1. The probability is distrbuted over min(max_index, length) indices,
 * where length is the length of the datastructure for which we need an index to query.
 */
int weighted_random_index(int max_index, int length, double log_base);

#endif