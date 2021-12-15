#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using uni_course_cpp::Graph;
using uni_course_cpp::GraphGenerationController;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::Logger;

constexpr int MIN_DEPTH = 0;
constexpr int MIN_NEW_VERTICES_COUNT = 0;
constexpr int MIN_GRAPHS_COUNT = 0;
constexpr int MIN_THREADS_COUNT = 1;

const std::string FILENAME_PREFIX = "graph_";
const std::string FILENAME_SUFFIX = ".json";

const int handle_depth_input() {
  int depth;
  std::cout << "Input depth:" << std::endl;
  while (!(std::cin >> depth) || (depth < MIN_DEPTH)) {
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return depth;
}

const int handle_new_vertices_count_input() {
  int new_vertices_count;
  std::cout << "Input new vertices count:" << std::endl;
  while (!(std::cin >> new_vertices_count) ||
         (new_vertices_count < MIN_NEW_VERTICES_COUNT)) {
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return new_vertices_count;
}

const int handle_graphs_count_input() {
  int graphs_count;
  std::cout << "Input graphs count:" << std::endl;
  while (!(std::cin >> graphs_count) || (graphs_count < MIN_GRAPHS_COUNT)) {
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return graphs_count;
}

const int handle_threads_count_input() {
  int threads_count;
  std::cout << "Input threads count:" << std::endl;
  while (!(std::cin >> threads_count) || (threads_count < MIN_THREADS_COUNT)) {
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return threads_count;
}

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string gen_started_string(int graph_number) {
  std::stringstream res;
  res << get_current_date_time() << ": Graph " << graph_number + 1
      << ", Generation Started\n";
  return res.str();
}

std::string gen_finished_string(int graph_number, const Graph& graph) {
  std::stringstream res;
  res << get_current_date_time() << ": Graph " << graph_number + 1
      << ", Generation Finished ";

  res << uni_course_cpp::graph_printing::print_graph_description(graph);

  res << "}\n}\n";
  return res.str();
}

void prepare_temp_directory() {
  std::filesystem::create_directory(
      uni_course_cpp::config::TEMP_DIRECTORY_PATH);
}

void write_to_file(const std::string& string, const std::string& filename) {
  std::ofstream filestream;
  filestream.open(filename);
  filestream << string;
  filestream.close();
}

std::vector<Graph> generate_graphs(const GraphGenerator::Params& params,
                                   int graphs_count,
                                   int threads_count) {
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);

  auto& logger = Logger::get_instance();

  auto graphs = std::vector<Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(gen_started_string(index)); },
      [&logger, &graphs](int index, Graph graph) {
        logger.log(gen_finished_string(index, graph));
        graphs.push_back(graph);
        write_to_file(uni_course_cpp::graph_printing::print_graph(graph),
                      uni_course_cpp::config::TEMP_DIRECTORY_PATH +
                          FILENAME_PREFIX + std::to_string(index) +
                          FILENAME_SUFFIX);
      });

  return graphs;
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();
  prepare_temp_directory();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto graphs = generate_graphs(params, graphs_count, threads_count);

  return 0;
}
