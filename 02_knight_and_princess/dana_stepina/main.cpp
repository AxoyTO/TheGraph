#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"

// #include <iostream>
// #include <algorithm>
// #include <unordered_set>
// #include <utility>
// #include <vector>

// using std::cout;
// using std::endl;
#include <fstream>

void write_to_file(const std::string& graph_string, const std::string& file) {
  std::ofstream out(file);
  out << graph_string;
  out.close();
}

int main() {
  const Depth depth = 4;  //я знаю, что надо вводить эти данные, но так проще
                          //отлаживать программу
  const int new_vertices_num = 3;

  auto graph = generate_graph(depth, new_vertices_num);

  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();

  // std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
