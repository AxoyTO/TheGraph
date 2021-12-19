#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <sstream>
#include <vector>

#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

const std::string logger_start_string(int graph_number) {
  std::stringstream string;
  string << get_current_date_time() << ": Graph " << graph_number
         << ", Generation Started\n";
  return string.str();
}

const std::string logger_finish_string(int graph_number,
                                       const uni_cpp_practice::Graph& graph) {
  std::stringstream string;
  string << get_current_date_time() << ": Graph " << graph_number
         << ", Generation Finished {\n";
  const auto& depths_map = graph.depths_map_;
  string << " depth: " << depths_map.size() - 1 << ",\n";

  std::stringstream vertices_string;
  vertices_string << "[";
  for (int i = 0; i < depths_map.size(); i++) {
    vertices_string << depths_map[i].size();
    if (i != depths_map.size() - 1) {
      vertices_string << ", ";
    }
  }
  vertices_string << "]";
  string << " vertices: " << graph.get_vertices().size() << ", "
         << vertices_string.str() << ",\n";

  std::stringstream edges_string;
  edges_string << "{";
  const auto& colors = graph.get_colors_map();
  for (auto it = colors.begin(); it != colors.end(); it++) {
    if (it != colors.begin()) {
      edges_string << ", ";
    }
    edges_string << uni_cpp_practice::GraphPrinter::color_to_string(it->first)
                 << ": " << (it->second).size();
  }
  edges_string << "}";
  string << " edges: " << graph.get_edges().size() << ", " << edges_string.str()
         << ",\n";

  string << "}\n\n";

  return string.str();
}

int handle_depth_input() {
  int depth;
  std::cout << "Enter the depth:" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Depth can't be negative" << std::endl;
    std::cout << "Enter the depth:" << std::endl;
    std::cin >> depth;
  }
  return depth;
}

int handle_new_vertices_num_input() {
  int new_vertices_num;
  std::cout << "Enter max amount of vertices genereted from one vertex:"
            << std::endl;
  std::cin >> new_vertices_num;
  while (new_vertices_num < 0) {
    std::cout
        << "Max amount of vertices genereted from one vertex can't be negative"
        << std::endl;
    std::cout << "Enter max amount of vertices genereted from one vertex:"
              << std::endl;
    std::cin >> new_vertices_num;
  }
  return new_vertices_num;
}

int handle_graphs_count_input() {
  int graphs_count;
  std::cout << "Enter number of graphs:" << std::endl;
  std::cin >> graphs_count;
  while (graphs_count < 0) {
    std::cout << "Number of graphs can't be negative" << std::endl;
    std::cout << "Enter number of graphs:" << std::endl;
    std::cin >> graphs_count;
  }
  return graphs_count;
}

int handle_threads_count_input() {
  int threads_count;
  std::cout << "Enter number of threads:" << std::endl;
  std::cin >> threads_count;
  while (threads_count < 0) {
    std::cout << "Number of threads can't be negative" << std::endl;
    std::cout << "Enter number of threads:" << std::endl;
    std::cin >> threads_count;
  }
  return threads_count;
}

uni_cpp_practice::Logger& prepare_logger() {
  std::filesystem::create_directory("temp");
  auto& logger = uni_cpp_practice::Logger::get_instance();
  logger.set_file("temp/log.txt");
  return logger;
}

void write_to_file(const std::string& string, const std::string& filename) {
  std::ofstream file;
  file.open(filename);

  if (file.is_open()) {
    file << string << std::endl;
    file.close();
  }
}

std::vector<uni_cpp_practice::Graph> generate_graphs(
    const uni_cpp_practice::GraphGenerator::Params& params,
    int graphs_count,
    int threads_count) {
  auto generation_controller = uni_cpp_practice::GraphGenerationController(
      threads_count, graphs_count, params);

  auto& logger = prepare_logger();

  auto graphs = std::vector<uni_cpp_practice::Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(logger_start_string(index + 1)); },
      [&logger, &graphs](int index, uni_cpp_practice::Graph graph) {
        logger.log(logger_finish_string(index + 1, graph));
        graphs.push_back(graph);
        const uni_cpp_practice::GraphPrinter graph_printer(graph);
        write_to_file(graph_printer.print(),
                      "temp/graph_" + std::to_string(index + 1) + ".json");
      });

  return graphs;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();

  const uni_cpp_practice::GraphGenerator::Params params(depth,
                                                        new_vertices_num);

  const auto graphs = generate_graphs(params, graphs_count, threads_count);

  return 0;
}
