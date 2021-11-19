#pragma once

#include <string>

const std::string JSON_GRAPH_FILENAME = "temp/graph_";

class Graph;

void write_graph(const Graph& graph, const int& graph_num);
