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

namespace uni_cpp_practice {

namespace graph_printing {

constexpr int COLORS_NUMBER = 6;
const std::string JSON_GRAPH_FILENAME = "temp/graph_";

using std::to_string;

void write_graph(const Graph& graph, const int& graph_num) {
  std::ofstream out;
  const std::string filename =
      JSON_GRAPH_FILENAME + std::to_string(graph_num) + ".json";
  out.open(filename, std::ofstream::out | std::ofstream::trunc);
  out << graph.to_json();
  out.close();
}

const std::string get_datetime() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

void write_log(Graph& work_graph,
               const int& depth,
               const int& new_vertices_num,
               const int& graph_num,
               Logger& logger) {
  std::string res = get_datetime();
  res += ": Graph " + to_string(graph_num) + ", Generation Started\n";
  res += get_datetime() + ": Graph " + to_string(graph_num) +
         ", Generation Ended {\n";
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
  std::array<int, COLORS_NUMBER> colors;
  for (int iter = 0; iter < COLORS_NUMBER; iter++)
    colors[iter] = 0;
  for (const auto& edge : work_graph.get_edges()) {
    switch (edge.color) {
      case Edge::Color::Gray: {
        colors[0]++;
        break;
      }
      case Edge::Color::Green: {
        colors[1]++;
        break;
      }
      case Edge::Color::Blue: {
        colors[2]++;
        break;
      }
      case Edge::Color::Yellow: {
        colors[3]++;
        break;
      }
      case Edge::Color::Red: {
        colors[4]++;
        break;
      }
      default:
        break;
    }
  }

  res += "gray: " + to_string(colors[0]) + ", ";
  res += "green: " + to_string(colors[1]) + ", ";
  res += "blue: " + to_string(colors[2]) + ", ";
  res += "yellow: " + to_string(colors[3]) + ", ";
  res += "red: " + to_string(colors[4]) + "}\n";
  res += "}\n";

  logger.log(res);
}

}  // namespace graph_printing

}  // namespace uni_cpp_practice
