#include "config.hpp"
#include "game.hpp"
#include "game_generator.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_path.hpp"
#include "graph_printer.hpp"
#include "graph_traversal_controller.hpp"
#include "graph_traverser.hpp"
#include "logger.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

using uni_course_cpp::Game;
using uni_course_cpp::GameGenerator;
using uni_course_cpp::Graph;
using uni_course_cpp::GraphGenerationController;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::GraphPath;
using uni_course_cpp::GraphTraversalController;
using uni_course_cpp::GraphTraverser;
using uni_course_cpp::Logger;
using uni_course_cpp::config::log_file_path;
using uni_course_cpp::graph_printing::print_game;
using uni_course_cpp::graph_printing::print_graph;
using uni_course_cpp::graph_printing::print_graph_description;
using uni_course_cpp::graph_printing::print_path;

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

void prepare_temp_directory() {
  std::filesystem::create_directory(uni_course_cpp::config::kTempDirectoryPath);
}

void write_to_file(const std::string& graph_json, const std::string& filename) {
  std::ofstream out(filename);
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

std::string traversal_started_string(int graph_num) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_num + 1) +
         ", Traversal Started\n";
}

std::string traversal_finished_string(int graph_number,
                                      const std::vector<GraphPath>& paths) {
  std::stringstream res;
  res << get_current_date_time() << ": Graph " << graph_number + 1
      << ", Traversal Finished, Paths: [";

  for (const auto& path : paths) {
    res << "\n  ";
    res << print_path(path);
    res << ",";
  }
  res << "\n]\n";
  return res.str();
}

void traverse_graphs(const std::vector<Graph>& graphs) {
  auto traversal_controller = GraphTraversalController(graphs);
  auto& logger = Logger::get_logger();

  traversal_controller.traverse(
      [&logger](int index, const Graph& graph) {
        logger.log(traversal_started_string(index));
      },
      [&logger](int index, const Graph& graph, std::vector<GraphPath> paths) {
        logger.log(traversal_finished_string(index, paths));
      });
}

std::string game_preparing_string() {
  return get_current_date_time() + " Game is preparing...\n";
}

std::string game_ready_string(const Game& game) {
  return get_current_date_time() + " Game is Ready " + print_game(game);
}

std::string shortest_path_searching_string() {
  return get_current_date_time() + " Searching for Shortest Path...\n";
}

std::string shortest_path_ready_string(const GraphPath& path) {
  return get_current_date_time() + " Shortest Path: " + print_path(path) + "\n";
}

std::string fastest_path_searching_string() {
  return get_current_date_time() + " Searching for Fastest Path...\n";
}

std::string fastest_path_ready_string(const GraphPath& path) {
  return get_current_date_time() + " Fastest Path: " + print_path(path) + "\n";
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  prepare_temp_directory();

  auto& logger = Logger::get_logger();
  logger.log(game_preparing_string());

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto game_generator = GameGenerator(params);
  const auto game = game_generator.generate();

  logger.log(game_ready_string(game));
  logger.log(shortest_path_searching_string());

  const auto shortest_path = game.find_shortest_path();

  logger.log(shortest_path_ready_string(shortest_path));
  logger.log(fastest_path_searching_string());

  const auto fastest_path = game.find_fastest_path();

  logger.log(fastest_path_ready_string(fastest_path));

  const auto map_json = print_graph(game.map());
  write_to_file(map_json, "map.json");

  return 0;
}
