#ifndef GARD_RANDOM_H
#define GARD_RANDOM_H

#include <random>
#include <stdlib.h>

using namespace std;

class GardRandom {

    public:
        mt19937 gen;

        GardRandom() {
            random_device rd;
            gen = mt19937(rd());
        }

        int get_random_int();

        int weighted_random_index(int max_index, int length, double log_base);
};

#endif