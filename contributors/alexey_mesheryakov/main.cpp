#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_json_printer.hpp"
#include "graph_path.hpp"
#include "graph_printer.hpp"
#include "graph_traversal_controller.hpp"
#include "graph_traverser.hpp"
#include "logger.hpp"

using uni_course_cpp::Depth;
using uni_course_cpp::Graph;
using uni_course_cpp::GraphGenerationController;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::GraphJsonPrinter;
using uni_course_cpp::GraphPath;
using uni_course_cpp::GraphPrinter;
using uni_course_cpp::GraphTraversalController;
using uni_course_cpp::GraphTraverser;
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

std::string traversal_started_string(int graph_number) {
  std::stringstream log_first_string;
  log_first_string << "Graph " + std::to_string(graph_number) +
                          ", Traversal Started\n";
  return log_first_string.str();
}

std::string traversal_finished_string(int graph_number,
                                      const std::vector<GraphPath>& paths) {
  std::stringstream log_second_string;
  log_second_string << "Graph " + std::to_string(graph_number) +
                           ", Traversal Finished, Paths: [\n";
  log_second_string << GraphPrinter::print_path(paths[0]);
  for (int path_index = 1; path_index < paths.size(); path_index++)
    log_second_string << ",\n" << GraphPrinter::print_path(paths[path_index]);

  log_second_string << "\n]\n";
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
  std::cout << "Input count of graphs:" << std::endl;
  std::cin >> graphs_count;
  while (graphs_count < 0) {
    std::cout << "Graph count must be be >= 0" << std::endl;
    std::cin >> graphs_count;
  }
  return graphs_count;
}

int handle_threads_count_input() {
  int threads_num;
  std::cout << "Enter the number of threads: " << std::endl;
  std::cin >> threads_num;
  while (threads_num < 1 || threads_num > std::thread::hardware_concurrency()) {
    std::cout << "number of threads must be > 0 and < "
              << std::thread::hardware_concurrency() << std::endl;
    std::cin >> threads_num;
  }
  return threads_num;
}

void write_to_file(const std::string& graph_output,
                   const std::string& file_name) {
  std::ofstream out_json;
  out_json.open(file_name, std::fstream::out | std::fstream::trunc);
  out_json << graph_output;
  out_json.close();
}

std::vector<Graph> generate_graphs(int graphs_count,
                                   int threads_count,
                                   const GraphGenerator::Params& params) {
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);

  auto& logger = Logger::get_instance();

  std::vector<Graph> graphs;

  generation_controller.generate(
      [&logger](int i) { logger.log(generation_started_string(i)); },
      [&logger, &graphs](int i, Graph&& graph) {
        const auto graph_printer = GraphPrinter(graph);
        const auto graph_json_printer = GraphJsonPrinter(graph);
        logger.log(generation_finished_string(i, graph_printer.print()));
        const std::string file_name =
            std::string(uni_course_cpp::config::kTempDirectoryPath) +
            std::to_string(i) + ".json";
        write_to_file(graph_json_printer.print(), file_name);
        graphs.push_back(graph);
      });
  return graphs;
}

void traverse_graphs(const std::vector<Graph>& graphs) {
  auto traversal_controller = GraphTraversalController(graphs);
  auto& logger = Logger::get_instance();

  traversal_controller.traverse(
      [&logger](int i, const Graph& graph) {
        logger.log(traversal_started_string(i));
      },
      [&logger](int i, const Graph& graph, std::vector<GraphPath> paths) {
        logger.log(traversal_finished_string(i, paths));
      });
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();
  std::filesystem::create_directory(uni_course_cpp::config::kTempDirectoryPath);
  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto graphs = generate_graphs(graphs_count, threads_count, params);
  traverse_graphs(graphs);

  return 0;
}
