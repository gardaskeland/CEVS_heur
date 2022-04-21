#include <pybind11/pybind11.h>
#include "solution_representation.h"
#include "graph.h"
#include "interface.h"
#include "read_file.h"

namespace py = pybind11;

using namespace std;

PYBIND11_MODULE(CEVS_heur, m) {
    py::class_<Graph>(m, "Graph")
        .def(py::init<vector<vector<int>> &>());

    py::class_<SolutionRepresentation>(m, "SolutionRepresentation")
        .def(py::init<int, int>());
        .def("print_solution", &SolutionRepresentation::print_solution());

    m.def("find_heuristic", &find_heuristic, "Uses the given heuristic to find
        a change to the graph that may be executed");
    
    m.def("execute_heuristic", &execute_heuristic, "Executes the last
        change found by find_heuristic");  

    m.def("read_gml", &read_gml, "Reads a gml file, returning a c++-object
        Feed this to Graph");
}  