
#include <iostream>
#include "test_solution_representation.h"
#include "test_ops/test_greedy_merge.h"
#include "test_ops/test_add_node.h"
#include "test_other/test_segment_tree.h"

using namespace std;

/**
 * Call test modules from here. If no FAIL is printed, all tests passed.
 */
int main() {
    cout << "Running tests\n";
    run_tests_solution_representation();
    run_tests_greedy_merge();
    run_tests_add_node();
    run_tests_segment_tree();
    cout << "Tests finished";
}