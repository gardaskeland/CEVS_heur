#ifndef B_PERTURBATION
#define B_PERTURBATION

#include <utility>
#include <tuple>
#include <set>

using namespace std;

class BPerturbation {
    public:
        int isolate_v;
        int isolate_si;
        tuple<int, int, int> to_add;
        int si_to_remove;

        pair<int, int> to_swap;
        pair<int, int> sets_to_swap;

        set<int> set_to_add;
};

#endif