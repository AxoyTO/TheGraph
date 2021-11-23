#include "graph.hpp"
#include "graph_generator.cpp"
#include "graph_printer.hpp"
#include "print_graph_json.cpp"

// #include <iostream>
// #include <algorithm>
// #include <unordered_set>
// #include <utility>
// #include <vector>

// using std::cout;
// using std::endl;

int main() {
  const DepthGraph depth = 4;
  const int new_vertices_num = 3;

  auto graph = Graph();

  generate_graph(graph, depth, new_vertices_num);
  write_graph_json_file(graph);

  return 0;
}
