#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "graph.hpp"
#include "graph_generator.hpp"

Graph task_03_get_graph(const int depth, const int new_vertices_num) {
  if (depth < 0) {
    throw std::invalid_argument("invalid depth argument");
  }
  if (new_vertices_num < 0) {
    throw std::invalid_argument("invalid new_vertices_num argument");
  }

  GraphGenerator graph_generator(depth, new_vertices_num);
  Graph ans = std::move(graph_generator.generate());

  if (ans.max_depth() < (size_t)depth) {
    std::cerr << "generated graph's depth=" << ans.max_depth()
              << " is less than specified one =" << depth << std::endl;
  }
  return ans;
}

int main() {
  Graph task_03_graph;
  int depth;
  int new_vertices_num;
  while (true) {
    std::cout << "Input the depth parameter: " << std::flush;
    std::cin >> depth;
    std::cout << "Input the amount of new vertices: " << std::flush;
    std::cin >> new_vertices_num;
    try {
      task_03_graph = task_03_get_graph(depth, new_vertices_num);
      break;
    } catch (const std::invalid_argument& error) {
      std::cout << error.what() << std::endl
                << "Please, redo the input:" << std::endl;
    }
  }

  const std::string json_string = task_03_graph.get_json_string();
  std::fstream json_file;
  json_file.open("graph.json", std::ios::out);
  if (!json_file.is_open()) {
    throw std::runtime_error("file not created");
  }
  json_file << json_string << std::endl;
  json_file.close();
  return 0;
}
