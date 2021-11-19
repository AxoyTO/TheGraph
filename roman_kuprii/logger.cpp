#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "graph.hpp"
#include "logger.hpp"

using std::to_string;

void write_log(Graph& work_graph,
               std::ofstream& logfile,
               const int& depth,
               const int& new_vertices_num,
               const int& graph_num,
               const Logger& logger) {
  std::string res =
      "time Graph " + to_string(graph_num) + ", Generation Started\n";
  res += "time Graph " + to_string(graph_num) + ", Generation Ended {\n";
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
      case Color::Gray: {
        colors[0]++;
        break;
      }
      case Color::Green: {
        colors[1]++;
        break;
      }
      case Color::Blue: {
        colors[2]++;
        break;
      }
      case Color::Yellow: {
        colors[3]++;
        break;
      }
      case Color::Red: {
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
