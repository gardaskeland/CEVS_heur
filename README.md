# CEVS_DRLH
An ALNS-based heuristic algorithm for solving the problem Cluster Editing with Vertex Splitting with inclusive vertex split formulated
by Abu-Khzam et al (doi: 10.1007/978-3-319-96151-4_1).

The implementation of the algorithm will be described in my thesis that will be published at the end of 2022.

Bonus: Branching algorithm for solving CEVS exactly in time bounded by maximum vertex degree is available in CEVS_heur/branching_CEVS/solve_cevs.cpp.
Compile together with header-file and it is ready to go. Will likely terminate only for small graphs.

## Usage
Requirements: gcc-compiler and make.

In the folder CEVS_heur/CEVS, build by writing the following in cmd:
make

Then run the algorithm for a graph by calling for example:
./cevs -iterations 10000 -runs 1 -file example_files/testfile.gr

The path to the graph file is relative to the folder CEVS_heur/CEVS.

The file formats .gml and .gr are supported. Reasonable results can only be expected for simple undirected graphs.

### The gml-file format
Look at the file CEVS_heur/CEVS/example_files/FARZ_0_test.gml. It is quite self-explanatory.

### The gr-file format
This is the file format used for the datasets given for the competition Parameterized Algorithms and Computational Experiments 2021 (PACE2021),
used for the problem of Cluster Editing. First line contains two tokens that can be ignored (p cep), the number of nodes and the number of edges (m)
in the graph. The following m lines contain the edges of the graph.