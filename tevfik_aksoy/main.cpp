#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

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

std::vector<std::pair<uni_cpp_practice::Edge::Color, int>> set_colors(
    const uni_cpp_practice::Graph& graph) {
  std::vector<std::pair<uni_cpp_practice::Edge::Color, int>> colors = {
      {uni_cpp_practice::Edge::Color::Gray,
       graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Gray)},
      {uni_cpp_practice::Edge::Color::Green,
       graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Green)},
      {uni_cpp_practice::Edge::Color::Blue,
       graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Blue)},
      {uni_cpp_practice::Edge::Color::Yellow,
       graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Yellow)},
      {uni_cpp_practice::Edge::Color::Red,
       graph.total_edges_of_color(uni_cpp_practice::Edge::Color::Red)}};

  return colors;
}

void log_start(uni_cpp_practice::Logger& logger, const int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Generation Started\n");
}

void log_depth(uni_cpp_practice::Logger& logger,
               const uni_cpp_practice::Graph& graph) {
  for (int j = 0; j <= graph.depth(); j++) {
    logger.log(std::to_string(graph.get_vertices_in_depth(j).size()));
    if (j != graph.depth())
      logger.log(", ");
  }
}

void log_colors(
    uni_cpp_practice::Logger& logger,
    const std::vector<std::pair<uni_cpp_practice::Edge::Color, int>>& colors) {
  for (int j = 0; j < colors.size(); j++) {
    logger.log(color_to_string(colors[j].first) + ": " +
               std::to_string(colors[j].second));
    if (j + 1 != colors.size())
      logger.log(", ");
  }
}

void log_end(uni_cpp_practice::Logger& logger,
             const uni_cpp_practice::Graph& graph,
             int graph_number) {
  logger.log(get_date_and_time() + ": Graph " + std::to_string(graph_number) +
             ", Generation Finished {  \n");
  logger.log("  depth: " + std::to_string(graph.depth()) + ",\n");
  logger.log("  vertices: " + std::to_string(graph.get_vertices().size()) +
             ", [");
  log_depth(logger, graph);
  logger.log("],\n  edges: " + std::to_string(graph.get_edges().size()) +
             ", {");
  const auto colors = set_colors(graph);
  log_colors(logger, colors);
  logger.log("}\n}\n");
}

void write_to_file(const uni_cpp_practice::GraphPrinter& graph_printer,
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
  const auto graph_generator =
      uni_cpp_practice::GraphGenerator(max_depth, new_vertices_num);
  auto& logger = uni_cpp_practice::Logger::get_instance();
  logger.set_file("./temp/log.txt");

  for (int i = 0; i < graphs_count; i++) {
    log_start(logger, i);
    const auto graph = graph_generator.generate();
    log_end(logger, graph, i);
    const auto graph_printer = uni_cpp_practice::GraphPrinter(graph);
    write_to_file(graph_printer, "./temp/graph_" + std::to_string(i) + ".json");
  }
  return 0;
}
