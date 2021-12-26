#include <fstream>
#include <iostream>
#include "graph.hpp"
#include "graph_json_printing.hpp"

void json_to_file(const std::string& str) {
  std::ofstream json;
  json.open("graph.json");
  if (!json.is_open()) {
    throw std::runtime_error("Can't open graph.json file");
  }
  json << str;
  json.close();
}

int handle_depth_input() {
  int depth = 0;
  std::cout << "Please, enter graph's depth:\n";
  std::cin >> depth;
  if (depth < 1) {
    throw std::runtime_error("Incorrect depth! Depth should be above zero!\n");
  }
  return depth;
}

int handle_new_vertices_count_input() {
  int count = 0;
  std::cout << "Please, enter graph's new vertices count:\n";
  std::cin >> count;
  if (count < 1) {
    throw std::runtime_error("Incorrect count! Count should be above zero!\n");
  }
  return count;
}

const GraphGenerator::Params GraphGenerationParams(
    const int depth,
    const int new_vertices_count) {
  return GraphGenerator::Params(depth, new_vertices_count);
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();

  const auto params = GraphGenerationParams(depth, new_vertices_count);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();

  const auto graph_json = printing::json::graph_to_string(graph);
  std::cout << graph_json << std::endl;
  json_to_file(graph_json);

  return 0;
}
