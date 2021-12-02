#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"

#include <fstream>
#include <iostream>

using Params = GraphGenerator::Params;

int handle_depth_input() {
  int entered_depth = -1;

  while (entered_depth < 0) {
    std::cout << "Enter depth value: ";
    std::cin >> entered_depth;
    if (entered_depth < 0)
      std::cout << "\nIncorrect depth: " << entered_depth
                << "\nDepth cannot be negative.\n";
    std::cout << std::endl;
  }

  return entered_depth;
}

int handle_new_vertices_num_input() {
  int entered_new_vertices_num = -1;

  while (entered_new_vertices_num < 0) {
    std::cout << "Enter the number of new vertices: ";
    std::cin >> entered_new_vertices_num;
    if (entered_new_vertices_num < 0)
      std::cout
          << "\nIncorrect value of the number of new vertices: "
          << entered_new_vertices_num
          << "\nThe value of the number of new vertices cannot be negative.\n";
    std::cout << std::endl;
  }

  return entered_new_vertices_num;
}

void write_to_file(const std::string& graph_string, const std::string& file) {
  std::ofstream out(file);
  out << graph_string;
  out.close();
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();

  const auto params = Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();

  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
