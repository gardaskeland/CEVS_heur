#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <set>


using namespace std;

template <typename T>

/**
 * @brief Segment tree data structure. Allows range queries over n time steps. Is updated/filled in
 * in order from first time step to last time step.
 * 
 */
class SegmentTree {
    private:
        vector<set<T>> tree;
        //The number of time steps covered by the data structure.
        int size;
        //Contains the index of the tree data structure that we fill in the next time step.
        int next_to_fill;

    public:
    //Initialises a segment tree of size n.
    SegmentTree(int n) {
        size = n;
        vector<set<T>> vec(2*n);
        tree = vec;
        next_to_fill = n;
    }

    SegmentTree() {
        size = 1;
        vector<set<T>> vec(2*1);
        tree = vec;
        next_to_fill = size;
    }

    //Fill the elements of vec for the next time step.
    void update(vector<T> &vec);
    
    void update(T &elm);

    void update_time(vector<T> &vec, int &time_step);

    void update_time(T &elm, int &time_step);

    //Query for the elements added between time steps left and right inclusively.
    set<T> query(int left, int right);

    //Returns the number of time steps the data structure covers.
    void get_size();
};

#endif