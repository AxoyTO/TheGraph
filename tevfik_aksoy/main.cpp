#include <array>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using Graph = uni_cpp_practice::Graph;
using Edge = uni_cpp_practice::Edge;
using GraphPrinter = uni_cpp_practice::GraphPrinter;
using GraphGenerator = uni_cpp_practice::GraphGenerator;
using Logger = uni_cpp_practice::Logger;

std::string get_date_and_time() {
  std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&now), "%d/%m/%Y %T");
  return date_time_string.str();
}

int handle_depth_input() {
  int max_depth = 0;
  std::cout << "Enter max_depth: ";
  do {
    std::cin >> max_depth;
    if (max_depth < 0)
      std::cerr << "Depth can not be negative!\n"
                   "Enter a non-negative max_depth: ";
  } while (max_depth < 0);
  return max_depth;
}

int handle_new_vertices_num_input() {
  int new_vertices_num = 0;
  std::cout << "Enter new_vertices_num: ";
  do {
    std::cin >> new_vertices_num;
    if (new_vertices_num < 0)
      std::cerr << "Number of new vertices created by each vertex can not be "
                   "negative!\nEnter a non-negative new_vertices_num: ";
  } while (new_vertices_num < 0);
  return new_vertices_num;
}

int handle_graphs_count_input() {
  int graphs_count = 0;
  std::cout << "Enter graphs_count: ";
  do {
    std::cin >> graphs_count;
    if (graphs_count < 0)
      std::cerr << "Count of graphs to be created can not be negative!\n"
                   "Enter a non-negative graphs_count: ";
  } while (graphs_count < 0);
  return graphs_count;
}

void log_start(Logger& logger, const int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Generation Started\n");
}

void log_depth(Logger& logger, const Graph& graph) {
  for (int j = 0; j <= graph.depth(); j++) {
    logger.log(std::to_string(graph.get_vertices_in_depth(j).size()));
    if (j != graph.depth())
      logger.log(", ");
  }
}

void log_colors(Logger& logger, const Graph& graph) {
  const std::array<Edge::Color, 5> colors = {
      Edge::Color::Gray, Edge::Color::Green, Edge::Color::Blue,
      Edge::Color::Yellow, Edge::Color::Red};
  for (int i = 0; i < colors.size(); i++) {
    logger.log(uni_cpp_practice::color_to_string(colors[i]) + ": " +
               std::to_string(graph.get_colored_edges(colors[i]).size()));
    if (i + 1 != colors.size())
      logger.log(", ");
  }
}

void log_end(Logger& logger, const Graph& graph, int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Generation Finished {  \n");
  logger.log("  depth: " + std::to_string(graph.depth()) + ",\n");
  logger.log("  vertices: " + std::to_string(graph.get_vertices().size()) +
             ", [");
  log_depth(logger, graph);
  logger.log("],\n  edges: " + std::to_string(graph.get_edges().size()) +
             ", {");
  log_colors(logger, graph);
  logger.log("}\n}\n");
}

void write_to_file(const GraphPrinter& graph_printer,
                   const std::string& filename) {
  std::ofstream jsonfile(filename, std::ios::out);
  if (!jsonfile.is_open())
    throw std::runtime_error("Error while opening the JSON file!");
  jsonfile << graph_printer.print();
  jsonfile.close();
}

int main() {
  const int graphs_count = handle_graphs_count_input();
  const int max_depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const auto graph_generator = GraphGenerator(max_depth, new_vertices_num);
  auto& logger = Logger::get_instance();
  std::filesystem::create_directory("./temp");

  logger.set_file("./temp/log.txt");

  for (int i = 0; i < graphs_count; i++) {
    log_start(logger, i);
    const auto graph = graph_generator.generate();
    log_end(logger, graph, i);
    const auto graph_printer = GraphPrinter(graph);
    write_to_file(graph_printer, "./temp/graph_" + std::to_string(i) + ".json");
  }
  return 0;
}
