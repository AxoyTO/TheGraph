#pragma once

#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "graph.hpp"

void PrintandWrite(std::ostream& os, std::ostream& fs, const string& str) {
  os << str;
  fs << str;
}

struct Logger {
  string time_str;
  time_t now{};
  static int k;
  string logfilename = "./temp/logger.txt";

  void start() {
    std::ofstream logfile(logfilename, std::ios::app);
    if (!logfile.is_open())
      std::cerr << "Error while opening the file " << logfilename << "\n";
    now = time(nullptr);
    time_str = ctime(&now);
    time_str.pop_back();
    PrintandWrite(
        std::cout, logfile,
        time_str + ": Graph " + std::to_string(k) + ", Generation Started\n");
  }

  void end(const Graph& graph,
           const int max_depth,
           const int new_vertices_num) {
    std::ofstream logfile(logfilename, std::ios::app);
    if (!logfile.is_open())
      std::cerr << "Error while opening the file " << logfilename << "\n";
    std::vector<std::pair<string, int>> colors = {
        {"gray", graph.total_edges_of_color(Edge::Color::Gray)},
        {"green", graph.total_edges_of_color(Edge::Color::Green)},
        {"blue", graph.total_edges_of_color(Edge::Color::Blue)},
        {"yellow", graph.total_edges_of_color(Edge::Color::Yellow)},
        {"red", graph.total_edges_of_color(Edge::Color::Red)}};

    now = time(nullptr);
    time_str = ctime(&now);
    time_str.pop_back();
    PrintandWrite(std::cout, logfile,
                  time_str + ": Graph " + std::to_string(k++) +
                      ", Generation Finished {\n");
    PrintandWrite(std::cout, logfile,
                  "  depth: " + std::to_string(max_depth) + ",\n");
    PrintandWrite(
        std::cout, logfile,
        "  new_vertices_num: " + std::to_string(new_vertices_num) + ",\n");
    PrintandWrite(
        std::cout, logfile,
        "  vertices: " + std::to_string(graph.get_vertices().size()) + ", [");

    for (int i = 0; i <= max_depth; i++) {
      PrintandWrite(std::cout, logfile,
                    "depth_" + std::to_string(i) + "_" +
                        std::to_string(graph.get_vertices_in_depth(i).size()));
      if (i != max_depth)
        PrintandWrite(std::cout, logfile, ", ");
    }
    PrintandWrite(std::cout, logfile, "],\n");
    PrintandWrite(
        std::cout, logfile,
        "  edges: " + std::to_string(graph.get_edges().size()) + ", {");
    for (int i = 0; i < colors.size(); i++) {
      PrintandWrite(std::cout, logfile,
                    colors[i].first + ": " + std::to_string(colors[i].second));
      if (i + 1 != colors.size())
        PrintandWrite(std::cout, logfile, ", ");
    }
    PrintandWrite(std::cout, logfile, "}\n}\n");
  }
};

int Logger::k = 1;