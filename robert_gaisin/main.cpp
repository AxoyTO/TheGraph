#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

#include <chrono>
//#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::Logger;
using uni_cource_cpp::config::log_file_path;
using uni_cource_cpp::graph_printing::print_graph;
using uni_cource_cpp::graph_printing::print_graph_description;
using uni_course_cpp::GraphGenerationController;

constexpr int INVALID_NEW_DEPTH = -1;
constexpr int INVALID_NEW_VERTICES_NUMBER = -1;
constexpr int INVALID_GRAPHS_NUMBER = 0;
constexpr int INVALID_THREADS_NUMBER = 0;

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

int handle_threads_count_input() {
  int threads_count = INVALID_THREADS_NUMBER;
  do {
    std::cout << "Enter threads number from zero: ";
    std::cin >> threads_count;
  } while (threads_count <= INVALID_THREADS_NUMBER);
  return threads_count;
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
  int graphs_quantity = INVALID_GRAPHS_NUMBER;
  do {
    std::cout << "Enter amount of graphs to generate: ";
    std::cin >> graphs_quantity;
  } while (graphs_quantity < INVALID_GRAPHS_NUMBER);
  return graphs_quantity;
}
/*
void prepare_temp_directory() {
  std::filesystem::create_directory(uni_cource_cpp::config::kTempDirectoryPath);
}*/

void write_to_file(const std::string& graph_json, const std::string& filename) {
  std::ofstream out(uni_cource_cpp::config::kTempDirectoryPath + filename);
  out << graph_json;
  out.close();
}

std::vector<Graph> generate_graphs(const GraphGenerator::Params& params,
                                   int graphs_count,
                                   int threads_count) {
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);

  auto& logger = Logger::get_logger();

  auto graphs = std::vector<Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(generation_started_string(index)); },
      [&logger, &graphs](int index, Graph graph) {
        logger.log(generation_finished_string(index, graph));
        graphs.push_back(graph);
        const auto graph_json = print_graph(graph);
        write_to_file(graph_json, "graph_" + std::to_string(index) + ".json");
      });

  return graphs;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();
  //prepare_temp_directory();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto graphs = generate_graphs(params, graphs_count, threads_count);

  return 0;
}
