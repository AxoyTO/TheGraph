#include <fstream>
#include <iostream>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
using uni_course_cpp::Depth;
using uni_course_cpp::Graph;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::GraphPrinter;

Depth handle_depth_input() {
  Depth depth;
  std::cout << "Enter the Depth of the graph" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> depth;
  }
  return depth;
}

int handle_new_vertices_num_input() {
  int new_vertices_num;
  std::cout << "Enter the Number of new vertices from each vertex of the graphs"
            << std::endl;
  std::cin >> new_vertices_num;
  while (new_vertices_num < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> new_vertices_num;
  }
  return new_vertices_num;
}

int main() {
  const auto new_vertices_num = handle_new_vertices_num_input();
  const Depth depth = handle_depth_input();
  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate_graph();
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();
  std::ofstream file;
  file.open("/home/alex/prac/graph_task/graph.json",
            std::fstream::out | std::fstream::trunc);
  file << graph_json;
  file.close();
  return 0;
}
