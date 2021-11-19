#pragma once

#include <string>

namespace uni_cpp_practice {

constexpr int COLORS_NUMBER = 6;
const std::string JSON_GRAPH_FILENAME = "temp/graph_";

class Graph;
class Logger;

const std::string get_datetime();

void write_graph(const Graph& graph, const int& graph_num);

void write_log(Graph& work_graph,
               std::ofstream& logfile,
               const int& depth,
               const int& new_vertices_num,
               const int& graph_num,
               const Logger& logger);

}  // namespace uni_cpp_practice
