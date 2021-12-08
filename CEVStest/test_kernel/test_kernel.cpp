#include "test_kernel.h"


//Still not implemented functionality to reduce clusters given k.
//So far only uses the methods of the kernel to find a smaller graph.
void test_kernel() {
    //Graph from "A more effective kernelization for cluster editing" by Jiong Guo, Elsevier, 2008.
    vector<vector<int>> adj = read_gz_file("../CEVStest/test_graphs/g7.txt");
    Graph g = Graph(adj);
    RevertKernel revert;
    int k = 100;
    
    WeightedGraph wg = find_kernel(g, k, revert);
    /**
    for (int i = 0; i < wg.n; i++) {
        cout << "edges from vertex " << i << ":\n";
        for (int j : wg.adj[i]) {
            cout << j << " ";
        }
        cout << "\n";
    }
    */

    if (wg.n != 7) {
        cout << "FAIL test_kernel: The number of nodes in kernel of g7 is wrong: " << wg.n << "\n";
    }
    adj = read_gz_file("../CEVStest/test_graphs/g8.txt");
    g = Graph(adj);
    RevertKernel revert2;
    k = 100;
    wg = find_kernel(g, k, revert2);
    /**
    for (int i = 0; i < wg.n; i++) {
        cout << "edges from vertex " << i << ":\n";
        for (int j : wg.adj[i]) {
            cout << j << " ";
        }
        cout << "\n";
    }
    */
    if (wg.n != 10) {
        cout << "FAIL test_kernel: The number of nodes in kernel of g8 is wrong " << wg.n << "\n";
    }
    vector<set<int>> connected_components = find_connected_components(wg);
    if (connected_components.size() != 2) {
        cout << "FAIL test_kernel: The number of components in kernel of g8 is wrong: " << wg.n << "\n"; 
    }

}

void run_tests_kernel() {
    cout << "run tests kernel\n";
    test_kernel();
}