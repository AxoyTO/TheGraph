#pragma once

#include <string>

namespace uni_cpp_practice {

class Graph;
class Logger;

namespace graph_printing {

constexpr int COLORS_NUMBER = 6;
const std::string JSON_GRAPH_FILENAME = "temp/graph_";

const std::string get_datetime();

void write_graph(const Graph& graph, const int& graph_num);

void write_log(Graph& work_graph,
               const int& depth,
               const int& new_vertices_num,
               const int& graph_num,
               Logger& logger);

}  // namespace graph_printing

}  // namespace uni_cpp_practice
