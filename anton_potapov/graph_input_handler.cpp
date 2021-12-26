#include <iostream>

#include "graph_input_handler.hpp"

namespace uni_cource_cpp {
int GraphInputHandler::handle_depth_input() {
  while (true) {
    std::cout << "Input the depth parameter: " << std::flush;
    int depth;
    std::cin >> depth;
    if (depth >= 0) {
      return depth;
    }
    std::cout << "depth should be > 0, please, redo the input:" << std::endl;
  }
}

int GraphInputHandler::handle_new_vertices_count_input() {
  while (true) {
    std::cout << "Input the amount of new vertices: " << std::flush;
    int new_vertices_num;
    std::cin >> new_vertices_num;
    if (new_vertices_num >= 0) {
      return new_vertices_num;
    }
    std::cout << "new vertices num should be > 0, please, redo the input:"
              << std::endl;
  }
}

int GraphInputHandler::handle_graphs_count_input() {
  while (true) {
    std::cout << "Input the amount of graphs to be generated: " << std::flush;
    int graphs_count;
    std::cin >> graphs_count;
    if (graphs_count >= 0) {
      return graphs_count;
    }
    std::cout << "new vertices num should be > 0, please, redo the input:"
              << std::endl;
  }
}

int GraphInputHandler::handle_threads_count_input() {
  while (true) {
    std::cout << "Input the amount of threads to be used: " << std::flush;
    int threads_count;
    std::cin >> threads_count;
    if (threads_count >= 0) {
      return threads_count;
    }
    std::cout << "threads count should be > 0, please, redo the input:"
              << std::endl;
  }
}
}  // namespace uni_cource_cpp
