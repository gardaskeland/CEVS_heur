#ifndef READ_FILE_H
#define READ_FILE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

vector<vector<int>> read_gz_file(string filename);

/**
 * Returns adjacency list of an undirected graph given by edges in a csv-file.
 * That is: Every row contains two integers representing vertices connected by an edge.
 */
vector<vector<int>> read_csv_graph(string &filename, int num_nodes);

/**
 * Reads a file in the graph modelling language and returns an adjacency list of the graph.
 */
vector<vector<int>> read_gml(string &filename);

/**
 * Read csv file of the karate graph.
 */
map<int, set<int>> read_csv_groups_karate(string &filename);

map<int, set<int>> read_csv_groups_football(string &filename);

/**
 * Read a solution written as csv with each line representing a cluster. 0 to n-1 enumeration.
 */
map<int, set<int>> read_solution(string &filename);


#endif