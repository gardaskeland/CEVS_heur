#include "remove_add.h"

#define tri tuple<int, int, int>

struct Modifications {
    vector<pair<int, int>> nodes_removed_from_sets;
};

optional<int> remove_add_3(Graph &g, SolutionRepresentation &sol) {
    BRemoveAdd &b = sol.book.b_remove_add;
    //do precomputations for operator
    if (!b.initiated) {
        g.find_all_p3s();
        b.best_p3s = vector<pair<int, tri>>(g.all_p3s.size());
        b.next_move = vector<tri>(3);
        b.initiated = true;
    }

    if (g.all_p3s.size() == 0) return {};

    if (b.counter == 0 || b.index >= b.best_p3s.size()) {
        int cost;
        int i = 0;
        Modifications mod;
        for (tri t : g.all_p3s) {
            int cost = remove_3(g, sol, t, mod);
            cost += add_3(g, sol, mod);
            b.best_p3s[i++] = make_pair(cost, t);
        }
        b.counter = b.best_p3s.size() / 2;
        b.index = 0;
    }

    tri p3 = b.best_p3s[b.index++].second;
    Modifications mod;
    int cost = remove_3(g, sol, p3, mod);
    //b.next move updated in add
    cost += add_3(g, sol, p3, mod);
    return cost;
}   