#include "weighted_random.h"

int weighted_random_index(int max_index, int length, double log_base) {
    int k = min(max_index, length);
    //cout << "k= " << k <<"\n";
    int r = rand() % ((int)pow(log_base,(k - 1))) + 1;
    //Using the change of log-base formula, giving us log_(log_base)(r).
    int floor_log = floor(log2(r)/log2(log_base));
    return k - floor_log - 1;
}
