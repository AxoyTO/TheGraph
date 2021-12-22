#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

constexpr int DEPTH_MIN = 0;
constexpr int NEW_VERTICES_MIN = 0;
constexpr int GRAPHS_COUNT_MIN = 0;

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string gen_started_string(int graph_number) {
  std::stringstream log_first_string;
  log_first_string << get_current_date_time() << ": Graph " << graph_number
                   << ", Generation Started";
  return log_first_string.str();
}

std::string gen_finished_string(int graph_number,
                                const uni_cource_cpp::Graph& graph) {
  const auto graph_printer = uni_cource_cpp::GraphPrinter();
  std::stringstream log_second_string;
  log_second_string << get_current_date_time() << ": Graph " << graph_number
                    << ", Generation Ended {\n";

  log_second_string << "  depth: " << graph.get_depth() << ",\n";
  log_second_string << "  vertices: " << graph.get_vertices_cnt() << ", [";

  log_second_string << graph.get_depth_map()[0].size();
  for (auto i = graph.get_depth_map().begin() + 1;
       i != graph.get_depth_map().end(); i++) {
    log_second_string << ", " << i->size();
  }

  log_second_string << "],\n"
                    << "  edges: " << graph.get_edges_cnt() << ", {";

  const auto colors = std::array<uni_cource_cpp::Edge::Color, 5>{
      uni_cource_cpp::Edge::Color::Gray, uni_cource_cpp::Edge::Color::Green,
      uni_cource_cpp::Edge::Color::Blue, uni_cource_cpp::Edge::Color::Yellow,
      uni_cource_cpp::Edge::Color::Red};
  for (const auto& color : colors) {
    log_second_string << graph_printer.print_edge_color(color) << ": "
                      << graph.get_colored_edges(color).size();
    if (&color != colors.end() - 1) {
      log_second_string << ", ";
    }
  }
  log_second_string << "}\n}\n";
  return log_second_string.str();
}

int handle_depth_input() {
  int depth;
  do {
    std::cout << "Input depth of graph:\n";
    std::cin >> depth;
    if (depth < DEPTH_MIN)
      std::cout << "Depth must be >= 0\n";
  } while (depth < DEPTH_MIN);
  return depth;
}

int handle_new_vertices_num_input() {
  int new_vertices_num = 0;
  do {
    std::cout << "Input number of vertecies from each vertex:\n";
    std::cin >> new_vertices_num;
    if (new_vertices_num < NEW_VERTICES_MIN)
      std::cout << "New_vertices_num must be >= 0\n";
  } while (new_vertices_num < NEW_VERTICES_MIN);
  return new_vertices_num;
}

int handle_graphs_count_input() {
  int graphs_count;
  do {
    std::cout << "Input count of graphs:\n";
    std::cin >> graphs_count;
    if (graphs_count < GRAPHS_COUNT_MIN)
      std::cout << "Graph count must be be >= 0\n";
  } while (graphs_count < GRAPHS_COUNT_MIN);
  return graphs_count;
}

void write_to_file(const std::string& graph_output,
                   const std::string& file_name) {
  std::ofstream out_json(file_name);
  out_json << graph_output;
  out_json.close();
}

int main() {
  std::filesystem::create_directory("temp/");

  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();

  const auto graph_generator =
      uni_cource_cpp::GraphGenerator(depth, new_vertices_num);
  auto& logger = uni_cource_cpp::Logger::get_logger();

  logger.set_output_file_path("temp/log.txt");

  for (int i = 0; i < graphs_count; i++) {
    logger.log(gen_started_string(i));
    const auto graph = graph_generator.generate_graph();
    logger.log(gen_finished_string(i, graph));

    const auto graph_printer = uni_cource_cpp::GraphPrinter();
    write_to_file(graph_printer.print(graph),
                  "temp/graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
