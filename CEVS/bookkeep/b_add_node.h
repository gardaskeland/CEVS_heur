#ifndef B_ADD_NODE_H
#define B_ADD_NODE_H

#include <map>

using namespace std;


class BAddNode {
    public:
        //Stores the index of the node to be added to set si.
        int v;

        //Stores the index of the set node v is to be added to.
        int si;

        //Stores the relative out-degrees of sets. Mapped from set index to relative out-degree.
        map<int, int> relative_out_degrees;

        BAddNode() {

        }
};


#endif