#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "graph.hpp"
#include "graph_generator.hpp"

int main() {
  int depth;
  int new_vertices_num;
  while (true) {
    std::cout << "Input the depth parameter: " << std::flush;
    std::cin >> depth;
    std::cout << "Input the amount of new vertices: " << std::flush;
    std::cin >> new_vertices_num;
    if (depth < 0 || new_vertices_num < 0) {
      std::cout << (depth < 0 ? "depth should be > 0"
                              : "new vertices num should be > 0")
                << " ,please, redo the input:" << std::endl;
    } else {
      break;
    }
  }
  GraphGenerator graph_generator(depth, new_vertices_num);
  Graph graph = std::move(graph_generator.generate_graph());

  if (graph.max_depth() < (size_t)depth) {
    std::cerr << "generated graph's depth=" << graph.max_depth()
              << " is less than specified one =" << depth << std::endl;
  }

  const std::string json_string = graph.get_json_string();
  std::fstream json_file;
  json_file.open("graph.json", std::ios::out);
  if (!json_file.is_open()) {
    throw std::runtime_error("file not created");
  }
  json_file << json_string << std::endl;
  json_file.close();
  return 0;
}
