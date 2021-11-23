#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <utility>
#include <vector>
#include "graph.hpp"
#include "printer_graph.cpp"
#include "graph_generator.cpp"

using std::cout;
using std::endl;

//ВЫВОД В ФАЙЛ JSON
void write_graph_json_file(const Graph& graph) {
  std::ofstream out("graph.json");
  out << get_graph_string(graph);
  out.close();
}

int main() {
  const DepthGraph depth = 4;
  const int new_vertices_num = 1;

  auto graph = Graph();

  generate_graph(graph, depth, new_vertices_num);
  write_graph_json_file(graph);

  return 0;
}
