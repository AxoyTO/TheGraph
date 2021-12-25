#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_json_printer.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using uni_course_cpp::Depth;
using uni_course_cpp::Graph;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::GraphJsonPrinter;
using uni_course_cpp::GraphPrinter;
using uni_course_cpp::Logger;

std::string generation_started_string(int graph_number) {
  std::stringstream log_first_string;
  log_first_string << "Graph " << graph_number << ", Generation Started"
                   << std::endl;
  return log_first_string.str();
}

std::string generation_finished_string(int graph_number,
                                       std::string graph_string) {
  std::stringstream log_second_string;
  log_second_string << "Graph " << graph_number << ", Generation Ended {"
                    << std::endl
                    << graph_string;

  return log_second_string.str();
}

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

int handle_graphs_count_input() {
  int graphs_count;
  std::cout << "Input count of graphs:\n";
  std::cin >> graphs_count;
  while (graphs_count < 0) {
    std::cout << "Graph count must be be >= 0\n";
    std::cin >> graphs_count;
  }
  return graphs_count;
}

void write_to_file(const std::string& graph_output,
                   const std::string& file_name) {
  std::ofstream out_json;
  out_json.open(file_name, std::fstream::out | std::fstream::trunc);
  out_json << graph_output;
  out_json.close();
}

int main() {
  const auto new_vertices_num = handle_new_vertices_num_input();
  const Depth depth = handle_depth_input();
  const int graphs_count = handle_graphs_count_input();
  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  auto& logger = uni_course_cpp::Logger::get_instance();

  std::filesystem::create_directory(uni_course_cpp::config::kTempDirectoryPath);

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate_graph();

    const auto graph_printer = GraphPrinter(graph);
    logger.log(generation_finished_string(i, graph_printer.print()));

    const auto graph_json_printer = GraphJsonPrinter(graph);
    write_to_file(graph_json_printer.print(),
                  uni_course_cpp::config::kTempDirectoryPath +
                      std::to_string(i) + ".json");
  }
}
