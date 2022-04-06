#ifndef B_PERTURBATION
#define B_PERTURBATION

#include <utility>
#include <tuple>
#include <set>
#include <vector>

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

        int u_to_remove;
        int v_to_remove;

        vector<int> vertices_for_clique_splitter_option;
        int vector_index = -1;
};

#endif