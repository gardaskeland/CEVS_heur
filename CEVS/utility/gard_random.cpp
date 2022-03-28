#include "gard_random.h"

int GardRandom::weighted_random_index(int max_index, int length, double log_base) {
    int k = min(max_index, length);
    //cout << "k= " << k <<"\n";
    int r = get_random_int() % ((int)pow(log_base, k)) + 1;
    //Using the change of log-base formula, giving us log_(log_base)(r).
    int floor_log = floor(log2(r)/log2(log_base));
    return max(0, k - floor_log - 1);
}

int GardRandom::get_random_int() {
    return abs((int)gen());
}