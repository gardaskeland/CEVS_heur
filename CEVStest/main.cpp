
#include <iostream>
#include "test_solution_representation.h"
#include "test_ops/test_greedy_merge.h"

using namespace std;

/**
 * Call test modules from here. If no FAIL is printed, all tests passed.
 */
int main() {
    cout << "Running tests\n";
    run_tests_solution_representation();
    run_tests_greedy_merge();
    cout << "Tests finished";
}