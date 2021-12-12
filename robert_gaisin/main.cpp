#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>

using config::log_file_path;
using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerationParams;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::Logger;
using uni_cource_cpp::graph_printing::print_graph;
using uni_cource_cpp::graph_printing::print_graph_description;

constexpr int INVALID_NEW_DEPTH = -1;
constexpr int INVALID_NEW_VERTICES_NUMBER = -1;
constexpr int GRAPHS_NUMBER = 0;
const std::string FILENAME_LOG = "log.txt";

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

Logger& prepare_logger() {
  auto& logger = Logger::get_logger();
  logger.set_file(log_file_path() + FILENAME_LOG);
  return logger;
}

std::string generation_started_string(int graph_num) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_num + 1) +
         ", Generation Started";
}

std::string generation_finished_string(int graph_num, const Graph& graph) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_num + 1) +
         ", Generation Finished " + print_graph_description(graph);
}

int handle_depth_input() {
  int depth = INVALID_NEW_DEPTH;
  do {
    std::cout << "Enter generate graph depth from zero: ";
    std::cin >> depth;
  } while (depth <= INVALID_NEW_DEPTH);
  return depth;
}

int handle_new_vertices_count_input() {
  int new_vertices_num = INVALID_NEW_VERTICES_NUMBER;
  do {
    std::cout << "Enter new_vertices_num from zero: ";
    std::cin >> new_vertices_num;
  } while (new_vertices_num < INVALID_NEW_VERTICES_NUMBER);
  return new_vertices_num;
}

int handle_graphs_count_input() {
  int graphs_quantity = GRAPHS_NUMBER;
  do {
    std::cout << "Enter amount of graphs to generate: ";
    std::cin >> graphs_quantity;
  } while (graphs_quantity < GRAPHS_NUMBER);
  return graphs_quantity;
}

void prepare_temp_directory() {
  std::filesystem::create_directory(log_file_path());
}

void write_to_file(std::string graph_json, std::string filename) {
  std::ofstream out(log_file_path() + filename);
  out << graph_json;
  out.close();
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  const auto params = GraphGenerationParams(depth, new_vertices_count);
  const auto generator = GraphGenerator(params);
  auto& logger = prepare_logger();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate();
    logger.log(generation_finished_string(i, graph));

    const auto graph_json = print_graph(graph);
    write_to_file(graph_json, "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
