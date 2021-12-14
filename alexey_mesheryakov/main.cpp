#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"

int main() {
  int depth = 0, new_vertices_num = 0;
  std::cin >> depth >> new_vertices_num;
  const auto params = Graph_generator::Params(depth, new_vertices_num);
  const auto generator = Graph_generator(params);
  const auto graph = generator.generate_graph();
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();
  std::ofstream file;
  file.open("graph.json", std::fstream::out | std::fstream::trunc);
  file << graph_json;
  file.close();
  return 0;
}
