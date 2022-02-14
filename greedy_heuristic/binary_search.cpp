#include "binary_search.h"

optional<int> binary_search(vector<int> &vec, int u) {
    int n = vec.size();
    if (n == 0) return optional<int>();
    if (n == 1) {
        if (vec[0] == u) return optional<int>(0);
        else return optional<int>();
    }
    int mid;
    int beg = 0; int end = n-1;
    //cout << "new\n";
    while (end >= beg) {
        //cout << "beg, end = " << beg << ", " << end << "\n";
        mid = beg + ((end - beg) / 2);
        //cout << "mid: " << mid << "\n";
        if (vec[mid] == u) {
            return optional<int>(mid);
        }
        else if (vec[mid] > u) {
            end = mid - 1;
        } else {
            beg = mid + 1;
        }
        //if (mid < 0) {
        //    cout << "ERROR: mid < 0\n";
        //    return optional<int>();
        //}
    }
    return optional<int>();
}

void binary_insert(vector<int> &vec, int u) {
    int n = vec.size();
    if (n == 0) {
        vec.push_back(u);
        return;
    }
    if (n == 1) {
        if (vec[0] < u) {
            vec.insert(vec.begin()+1, u);
        } else {
            vec.insert(vec.begin(), u);
        }
        return;
    }
    int mid = 0;
    int beg = 0; int end = n-1;
    while (end >= beg) {
        mid = beg + ((end - beg) / 2);
        if (vec[mid] == u) {
            vec.insert(vec.begin()+mid, u);
            return;
        }
        //cout << "mid: " << mid << "\n";
        else if (vec[mid] > u) {
            end = mid - 1;
        } else {
            beg = mid + 1;
        } 
    }
    vector<int>::iterator it = vec.begin();
    if (vec[mid] < u) {
        vec.insert(it+mid+1, u);
    } else {
        vec.insert(it+mid, u);
    }
}