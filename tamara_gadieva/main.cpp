#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include "graph.hpp"

constexpr int DEPTH_MIN = 0;
constexpr int NEW_VERTICES_NUM_MIN = 0;

int handle_depth_input() {
  int depth = DEPTH_MIN - 1;
  while (depth < DEPTH_MIN) {
    std::cout << "Enter the depth of graph (int, >= 0): ";
    std::cin >> depth;
    if (std::cin.fail()) {
      std::cin.clear();
      std::string dummy;
      std::cin >> dummy;
      depth = DEPTH_MIN - 1;
      continue;
    }
  }
  return depth;
}

int handle_new_vertices_num_input() {
  int new_vertices_num = NEW_VERTICES_NUM_MIN - 1;
  while (new_vertices_num < NEW_VERTICES_NUM_MIN) {
    std::cout << "Enter the number of new vertices (int, >= 0): ";
    std::cin >> new_vertices_num;
    if (std::cin.fail()) {
      std::cin.clear();
      std::string dummy;
      std::cin >> dummy;
      new_vertices_num = NEW_VERTICES_NUM_MIN - 1;
      continue;
    }
  }
  return new_vertices_num;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();

  std::ofstream graph_json("graph.json");
  graph_json << graph.json_string() << std::endl;
  graph_json.close();

  return 0;
}
