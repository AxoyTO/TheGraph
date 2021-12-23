#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"
using uni_course_cpp::Depth;
using uni_course_cpp::Graph;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::GraphPrinter;
using uni_course_cpp::Logger;

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string generation_started_string(int graph_number) {
  std::stringstream log_first_string;
  log_first_string << get_current_date_time() << ": Graph " << graph_number
                   << ", Generation Started";
  return log_first_string.str();
}

std::string generation_finished_string(int graph_number,
                                       std::string graph_string) {
  std::stringstream log_second_string;
  log_second_string << get_current_date_time() << ": Graph " << graph_number
                    << ", Generation Ended {\n"
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

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate_graph();

    const auto graph_description = uni_course_cpp::printing::print_graph(graph);
    logger.log(generation_finished_string(i, graph_description));

    const auto graph_printer = GraphPrinter(graph);
    write_to_file(graph_printer.print(),
                  "temp/graph_" + std::to_string(i) + ".json");
  }
}
