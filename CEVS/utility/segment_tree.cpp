#include "segment_tree.h"

template <typename T>
void SegmentTree<T>::update(vector<T> &vec) {
    for(T elm : vec) {
        tree[next_to_fill].insert(elm);
    }
    int parent = next_to_fill / 2;
    while (parent > 1) {
        for (T elm : vec) {
            tree[parent].insert(elm);
        }
        parent = parent / 2;
    }
    next_to_fill += 1;

}

template <typename T>
void SegmentTree<T>::update_time(vector<T> &vec, int &time_step) {
    int real_time_step = size + time_step;
    for(T elm : vec) {
        tree[real_time_step].insert(elm);
    }
    int parent = real_time_step / 2;
    while (parent > 1) {
        for (T elm : vec) {
            tree[parent].insert(elm);
        }
        parent = parent / 2;
    }
    next_to_fill = real_time_step + 1;
}


template <typename T>
void SegmentTree<T>::update(T &elm) {
    tree[next_to_fill].insert(elm);
    int parent = next_to_fill / 2;
    while (parent > 1) {
        tree[parent].insert(elm);
        parent = parent / 2;
    }
    next_to_fill += 1;
}

template <typename T>
void SegmentTree<T>::update_time(T &elm, int &time_step) {
    int real_time_step = size + time_step;
    tree[real_time_step].insert(elm);
    int parent = real_time_step / 2;
    while (parent > 1) {
        tree[parent].insert(elm);
        parent = parent / 2;
    }
    next_to_fill = real_time_step + 1;
}

template <typename T>
set<T> SegmentTree<T>::query(int left, int right) {
    set<T> to_return;
    //if (left < 0) return to_return;
    //if (right < left) return to_return;
    int real_left = size + left;
    int real_right = size + right;
    for(T elm : tree[real_left]) {
        to_return.insert(elm);
    }
    for (T elm : tree[real_right]) {
        to_return.insert(elm);
    }


    while(real_left / 2 != real_right / 2) {
        if (real_left % 2 == 0) {
            for (T elm : tree[real_left + 1]) {
                to_return.insert(elm);
            }
        } 
        real_left = real_left / 2;

        if (real_right % 2 == 1) {
            for (T elm : tree[real_right - 1]) {
                to_return.insert(elm);
            }
        } 
        real_right = real_right / 2;
    }

    return to_return;
}

template class SegmentTree<int>;