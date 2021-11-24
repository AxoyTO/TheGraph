#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "graph.hpp"
#include "graph_printing.hpp"
#include "logger.hpp"

namespace {

const std::string JSON_GRAPH_FILENAME = "temp/graph_";

using std::to_string;

std::string get_datetime() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

}  // namespace

namespace uni_cpp_practice {

namespace logging_helping {

void write_graph(const Graph& graph, int graph_num) {
  std::ofstream out;
  const std::string filename =
      JSON_GRAPH_FILENAME + std::to_string(graph_num) + ".json";
  out.open(filename, std::ofstream::out | std::ofstream::trunc);
  out << graph_printing::graph_to_json(graph);
  out.close();
}

std::string write_log_start(int graph_num) {
  std::string res = get_datetime();
  res += ": Graph " + to_string(graph_num) + ", Generation Started";
  return res;
}

std::string write_log_end(Graph& work_graph,
                          int depth,
                          int new_vertices_num,
                          int graph_num) {
  std::string res = get_datetime();
  res += ": Graph " + to_string(graph_num) + ", Generation Ended {\n";
  res += "\tdepth: " + to_string(depth) + ",\n";
  res += "\tnew_vertices_num: " + to_string(new_vertices_num) + ",\n";
  res += "vertices: " + to_string(work_graph.get_vertices_num()) + ", [";

  std::vector<int> depth_count;
  for (int iter = 0; iter < work_graph.get_depth(); iter++)
    depth_count.emplace_back(0);
  for (const auto& vertex : work_graph.get_vertices()) {
    depth_count[vertex.depth]++;
  }
  for (const auto& depth : depth_count) {
    res += to_string(depth) + ", ";
  }
  res.pop_back();
  res.pop_back();
  res += "],\n";
  res += "edges: " + to_string(work_graph.get_edges_num()) + ", {";

  const auto colors = std::vector<Edge::Color>(
      {Edge::Color::Gray, Edge::Color::Green, Edge::Color::Blue,
       Edge::Color::Yellow, Edge::Color::Red});

  for (const auto& color : colors) {
    res += graph_printing::color_to_string(color) + ": " +
           to_string(work_graph.get_edge_ids_with_color(color).size()) + ", ";
  }

  res += "}\n";
  return res;
}

}  // namespace logging_helping

}  // namespace uni_cpp_practice
