#include <fstream>
#include <iostream>
#include <string>

#include "graph.hpp"
#include "graph_printer.hpp"

void write_graph(const Graph& graph, const int& graph_num) {
  std::ofstream out;
  const std::string filename =
      JSON_GRAPH_FILENAME + std::to_string(graph_num) + ".json";
  out.open(filename, std::ofstream::out | std::ofstream::trunc);
  out << graph.to_json();
  out.close();
}
