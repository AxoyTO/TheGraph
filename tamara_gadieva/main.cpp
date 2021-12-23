#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_printing.hpp"
#include "graph_traversal_controller.hpp"
#include "graph_traverser.hpp"
#include "logger.hpp"

using uni_cource_cpp::Edge;
using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerationController;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::GraphTraversalController;
using uni_cource_cpp::GraphTraverser;
using uni_cource_cpp::Logger;

constexpr int DEPTH_MIN = 0;
constexpr int NEW_VERTICES_NUM_MIN = 0;
constexpr int GRAPHS_NUM_MIN = 1;
constexpr int THREADS_NUM_MIN = 1;
const int THREADS_NUM_MAX = std::thread::hardware_concurrency();

int handle_depth_input() {
  int depth = DEPTH_MIN - 1;
  while (depth < DEPTH_MIN) {
    std::cout << "Enter the depth of graph (int, >= 0): ";
    std::cin >> depth;
    if (std::cin.fail()) {
      std::cin.clear();
      std::string dummy;
      std::cin >> dummy;
      depth = DEPTH_MIN - 1;
      continue;
    }
  }
  return depth;
}

int handle_new_vertices_num_input() {
  int new_vertices_num = NEW_VERTICES_NUM_MIN - 1;
  while (new_vertices_num < NEW_VERTICES_NUM_MIN) {
    std::cout << "Enter the number of new vertices (int, >= 0): ";
    std::cin >> new_vertices_num;
    if (std::cin.fail()) {
      std::cin.clear();
      std::string dummy;
      std::cin >> dummy;
      new_vertices_num = NEW_VERTICES_NUM_MIN - 1;
      continue;
    }
  }
  return new_vertices_num;
}

int handle_graphs_count_input() {
  int graphs_num = GRAPHS_NUM_MIN - 1;
  while (graphs_num < GRAPHS_NUM_MIN) {
    std::cout << "Enter the number of graphs (int, >= 0): ";
    std::cin >> graphs_num;
    if (std::cin.fail()) {
      std::cin.clear();
      std::string dummy;
      std::cin >> dummy;
      graphs_num = GRAPHS_NUM_MIN - 1;
      continue;
    }
  }
  return graphs_num;
}

int handle_threads_count_input() {
  int threads_num = THREADS_NUM_MIN - 1;
  while (threads_num < THREADS_NUM_MIN || threads_num > THREADS_NUM_MAX) {
    std::cout << "Enter the number of threads (int, >= 0): ";
    std::cin >> threads_num;
    if (std::cin.fail()) {
      std::cin.clear();
      std::string dummy;
      std::cin >> dummy;
      threads_num = THREADS_NUM_MIN - 1;
      continue;
    }
  }
  return threads_num;
}

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string generation_started_string(int graph_number) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_number) +
         ", Generation Started\n";
}

std::string generation_finished_string(int graph_number, const Graph& graph) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_number) +
         ", Generation Finished {\n" +
         graph_printing::graph_description(graph) + "}\n";
}

std::vector<Graph> generate_graphs(int graphs_count,
                                   int threads_count,
                                   const GraphGenerator::Params& params) {
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);

  auto& logger = Logger::get_instance();

  auto graphs = std::vector<Graph>(graphs_count);

  generation_controller.generate(
      [&logger](int i) { logger.log(generation_started_string(i)); },
      [&logger, &graphs](int i, Graph graph) {
        logger.log(generation_finished_string(i, graph));
        const std::string file_name =
            std::string(uni_cource_cpp::config::TEMP_FOLDER_PATH) + "graph_" +
            std::to_string(i) + ".json";
        std::ofstream graph_json(file_name);
        graph_json << graph_printing::graph_to_json_string(graph) << std::endl;
        graph_json.close();
        graphs[i] = std::move(graph);
      });
  return graphs;
}

std::string traversal_started_string(int graph_number) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_number) +
         ", Traversal Started\n";
}

std::string traversal_finished_string(
    int graph_number,
    const std::vector<GraphTraverser::GraphPath>& paths) {
  std::string result_string = get_current_date_time() + ": Graph " +
                              std::to_string(graph_number) +
                              ", Traversal Finished, Paths: [\n";

  for (auto paths_ind = paths.begin(); paths_ind != paths.end(); paths_ind++) {
    if (paths_ind != paths.begin()) {
      result_string += ",\n";
    }
    result_string += graph_printing::print_path(*paths_ind);
  }

  result_string += "\n]\n";
  return result_string;
}

void traverse_graphs(const std::vector<Graph>& graphs) {
  auto traversal_controller = GraphTraversalController(graphs);
  auto& logger = Logger::get_instance();

  traversal_controller.traverse(
      [&logger](int i, const Graph& graph) {
        logger.log(traversal_started_string(i));
      },
      [&logger](int i, const Graph& graph,
                std::vector<GraphTraverser::GraphPath> paths) {
        logger.log(traversal_finished_string(i, paths));
      });
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();
  std::filesystem::create_directory(uni_cource_cpp::config::TEMP_FOLDER_PATH);

  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto graphs = generate_graphs(graphs_count, threads_count, params);

  traverse_graphs(graphs);
  return 0;
}
