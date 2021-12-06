#include "test_segment_tree.h"


SegmentTree<int> make_segment_tree(string filename) {
    ifstream myfile(filename);
    string line;
    getline(myfile, line);
    istringstream iss(line);

    int n;
    iss >> n;

    SegmentTree<int> sgt(n);
    int elm;
    for (int i = 0; i < n; i++) {
        vector<int> to_add;
        getline(myfile, line);
        istringstream iss(line);
        while(!(iss >> elm)) {
            to_add.push_back(elm);
        }
        sgt.update(to_add);
    }

    return sgt;
}

void test_segment_tree() {
    SegmentTree<int> sgt = make_segment_tree("segment_tree.txt");
    set<int> res = sgt.query(1,5);
    vector<int> vec {3, 4, 5, 6, 7, 9, 10};
    for (int elm : vec) {
        if (!(res.find(elm) != res.end())) {
            cout << "FAIL test_segment_tree: element " << elm << " was not found in range 1 to 5.";
        }
    }
    if (res.find(2) != res.end()) {
        cout << "FAIL test_segment_tree: element 2 was found in range 1 to 5 but should not be there";
    }
    if (res.find(11) != res.end()) {
        cout << "FAIL test_segment_tree: elment 11 was found in range 1 to 5 but should not be there.";
    }

    res = sgt.query(6,6);
    vector<int> vec2 {11, 12};
    for (int elm : vec) {
        if (!(res.find(elm) != res.end())) {
            cout << "FAIL test_segment_tree: element " << elm << " was not found in range 6 to 6.";
        }
    }
    

}

void run_tests_segment_tree() {
    test_segment_tree();
}