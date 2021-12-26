#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"
#include "logger.hpp"

namespace {
const int handle_graphs_count_input() {
  int graph_count;
  std::cout << "Please, enter: how many graphs do you want: ";
  std::cin >> graph_count;
  return graph_count;
}

const int handle_depth_input() {
  int depth;
  std::cout << "Please, enter the depth of your graph: ";
  std::cin >> depth;
  return depth;
}

const int handle_new_vertices_count_input() {
  int new_vertices_count;
  std::cout
      << "Please, enter count of new neighbors for each vertex of your graph: ";
  std::cin >> new_vertices_count;
  return new_vertices_count;
}

const int handle_threads_count_input() {
  int threads_count;
  std::cout << "Please, enter count of threads: ";
  std::cin >> threads_count;
  return threads_count;
}

void write_to_file(const std::string& graph_json,
                   const std::string& file_path) {
  std::ofstream out_file;
  out_file.open(std::string(config::kTempDirectoryPath) + file_path);
  out_file << graph_json;
  out_file.close();
}

std::string generation_started_string(const int graph_number) {
  std::stringstream log_string;
  log_string << "Graph " << graph_number << " Generation Started" << std::endl;
  return log_string.str();
}

std::string generation_finished_string(const int graph_number,
                                       const std::string& graph_description) {
  std::stringstream log_string;
  log_string << "Graph " << graph_number << " Generation Finished {"
             << std::endl;
  log_string << graph_description << std::endl << "}" << std::endl;
  return log_string.str();
}

void prepare_temp_directory() {
  if (!std::filesystem::exists(config::kTempDirectoryPath) ||
      (std::filesystem::exists(config::kTempDirectoryPath) &&
       !std::filesystem::is_directory(config::kTempDirectoryPath))) {
    std::filesystem::create_directory(config::kTempDirectoryPath);
  }
}

std::vector<uni_course_cpp::Graph> generate_graphs(
    const uni_course_cpp::GraphGenerator::Params& params,
    int graphs_count,
    int threads_count) {
  auto generation_controller = uni_course_cpp::GraphGenerationController(
      threads_count, graphs_count, params);
  auto& logger = uni_course_cpp::Logger::get_logger();
  auto graphs = std::vector<uni_course_cpp::Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(generation_started_string(index)); },
      [&logger, &graphs](int index, uni_course_cpp::Graph graph) {
        const auto graph_description =
            uni_course_cpp::printing::print_graph(graph);
        logger.log(generation_finished_string(index, graph_description));
        graphs.push_back(graph);
        const auto graph_json =
            uni_course_cpp::printing::json::print_graph(graph);
        write_to_file(graph_json, "graph_" + std::to_string(index) + ".json");
      });

  return graphs;
}

}  // namespace

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();
  if (threads_count > std::thread::hardware_concurrency())
    throw std::runtime_error("Too much threads");

  prepare_temp_directory();

  const auto params =
      uni_course_cpp::GraphGenerator::Params(depth, new_vertices_count);
  const auto graphs = generate_graphs(params, graphs_count, threads_count);
  return 0;
}
