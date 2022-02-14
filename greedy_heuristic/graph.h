#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include "binary_search.h"

//#define pair pair<int, int>;

using namespace std;

class Graph {
    public:
        int n; //May be smaller than length of adj because of splits.
        //splits vertices remain with empty adjacencylist
        vector<vector<int>> adj;
        vector<vector<int>> forbid_permanent;
        //Stores the paths of vertices being split, each vertex pointing to itself if it is an
        //original vertex or the vertex it is split from if not
        vector<int> parents;

        /**
         * Return true if graph has edge.
         */
        bool has_edge(int u, int v);

        void add_edge(int u, int v);

        void remove_edge(int u, int v);

        /**
         * Splits the vertex v in to v1 and v2 so that u is connected only to v1 and w only to w2 (or opposite).
         * If |N(u) intersection N(v)| >= |N(w) intersection N(v)| we let N(v1) = (N[u] intersection N(v)) and 
         * N(v2) = N(v)\N(v1).
         */
        void split_vertex_(int u, int v, int w);

        void split_vertex(optional<tuple<int, int, int>> t);

        //Uses parents to find the original vertex u was split from.
        int original_node(int u);

        vector<int>& get_parents();

        vector<int>& get_forbid_permanent();

        void set_forbid_permanent(int u, int v);

        vector<int>& get_adj();

        Graph(int n_) {
            n = n_;
            for (int i = 0; i < n; i++) {
                adj.emplace_back(vector<int>());
                forbid_permanent.emplace_back(vector<int>(n, 0));
                parents.emplace_back(i);

            }
        }

        void print_graph();


};

#endif