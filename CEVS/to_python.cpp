#include <pybind11/pybind11.h>
#include "solution_representation.h"
#include "utility/shallow_solution.h"
#include "graph.h"
#include "interface.h"
#include "read_file.h"
#include <string>
#include <set>
#include <map>

namespace py = pybind11;

using namespace std;

PYBIND11_MODULE(CEVS_heur, m) {
    py::class_<Graph>(m, "Graph")
        .def(py::init<vector<vector<int>> &>())
        .def("get_number_nodes", &Graph::get_number_nodes, "Get the number of nodes in the graph");

    //Add possibility to read n
    py::class_<SolutionRepresentation>(m, "SolutionRepresentation")
        .def(py::init<int, int>())
        .def("print_solution", &SolutionRepresentation::print_solution)
        .def("initial_solution", &SolutionRepresentation::initial_solution)
        .def("deep_copy", &SolutionRepresentation::deep_copy)
        .def("get_shallow_solution", &SolutionRepresentation::get_shallow_solution);

    py::class_<ShallowSolution>(m, "ShallowSolution")
        .def(py::init<map<int, set<int>>, map<int, set<int>>>())
        .def("print_solution", &ShallowSolution::print_solution)
        .def("solution_as_string", &ShallowSolution::solution_as_string);

    m.def("find_heuristic", &find_heuristic, "Uses the given heuristic to find \
        a change to the graph that may be executed.");
    
    m.def("execute_heuristic", &execute_heuristic, "Executes the last \
        change found by find_heuristic.");

    m.def("make_graph_from_gml", &make_graph_from_gml, "Reads a gml file and returns a graph \
        corresponding to the content of the file.");

    m.def("make_graph_from_gr", &make_graph_from_gr, "Reads a gr file and returns a graph \
        corresponding to the content of the file.");
    
    m.def("objective_function", &objective_function, "Returns the cost of current solution.");
}  