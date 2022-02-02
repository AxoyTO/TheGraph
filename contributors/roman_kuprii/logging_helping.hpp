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
#include "graph_traverser.hpp"
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

std::string write_log_end(const Graph& work_graph, int graph_num) {
  std::string res = get_datetime();
  res += ": Graph " + to_string(graph_num) + ", Generation Ended {\n";
  res += "  depth: " + to_string(work_graph.get_depth()) + ",\n";
  res += "  vertices: " + to_string(work_graph.get_vertices().size()) + ", [";

  for (int depth = 0; depth <= work_graph.get_depth(); depth++) {
    res += to_string(work_graph.get_vertex_ids_at_depth(depth).size()) + ", ";
  }
  res.pop_back();
  res.pop_back();
  res += "],\n";
  res += "  edges: " + to_string(work_graph.get_edges().size()) + ", {";

  const auto colors = std::vector<Edge::Color>(
      {Edge::Color::Gray, Edge::Color::Green, Edge::Color::Blue,
       Edge::Color::Yellow, Edge::Color::Red});

  for (const auto& color : colors) {
    res += graph_printing::color_to_string(color) + ": " +
           to_string(work_graph.get_edge_ids_with_color(color).size()) + ", ";
  }
  res.pop_back();
  res.pop_back();
  res += "}\n}\n";
  return res;
}

std::string write_traverse_start(int graph_num) {
  std::string res = get_datetime();
  res += ": Graph " + to_string(graph_num) + ", Traversal Started";
  return res;
}

std::string write_traverse_end(
    int graph_num,
    const std::vector<GraphTraverser::Path>& pathes) {
  std::string res = get_datetime();
  res += ": Graph " + to_string(graph_num) + ", Traversal Finished, Paths: [\n";
  for (const auto& path : pathes) {
    res += "  ";
    res += graph_printing::path_to_json(path);
    res += ",\n";
  }
  if (pathes.size()) {
    res.pop_back();
    res.pop_back();
  }
  res += "\n]\n";
  return res;
}

}  // namespace logging_helping

}  // namespace uni_cpp_practice
