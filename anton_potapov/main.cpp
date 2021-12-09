#include <fstream>
#include <iostream>
#include <string>

#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"

int handle_depth_input() {
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

int handle_new_vertices_count_input() {
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

int handle_graphs_count_input() {
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

void prepare_temp_directory() {
  
}

void write_to_file(const std::string& file_text, const std::string& file_path) {
  std::fstream json_file;
  json_file.open(file_path, std::ios::out);
  if (!json_file.is_open()) {
    throw std::runtime_error("file not created");
  }
  json_file << file_text << std::endl;
  json_file.close();
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(params);
  // auto& logger = Logger::get_logger();

  for (int i = 0; i < graphs_count; i++) {
    // logger.log(generation_started_string(i));
    const auto graph = generator.generate_graph();
    if (graph.max_depth() < depth) {
    std::cerr << "generated graph's depth=" << graph.max_depth()
              << " is less than specified one =" << depth << std::endl;
    }
    // logger.log(generation_finished_string(i, graph));

    auto graph_printer = GraphPrinter(graph);
    const auto graph_json = graph_printer.print();
    write_to_file(graph_json, "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
