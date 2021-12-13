#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "logger.hpp"

using uni_cource_cpp::Edge;
using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::Logger;

constexpr int DEPTH_MIN = 0;
constexpr int NEW_VERTICES_NUM_MIN = 0;
constexpr int GRAPHS_NUM_MIN = 1;
constexpr int COLORS_NUM = 4;

int handle_depth_input();
int handle_new_vertices_num_input();
int handle_graphs_count_input();

std::string generation_started_string(int);
std::string generation_finished_string(int, const Graph&);
std::string get_current_date_time();

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();
  std::filesystem::create_directory(uni_cource_cpp::config::TEMP_FOLDER_PATH);

  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  auto& logger = Logger::get_instance();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate();
    logger.log(generation_finished_string(i, graph));
    const std::string file_name = "./temp/graph_" + std::to_string(i) + ".json";
    std::ofstream graph_json(file_name);
    graph_json << graph.json_string() << std::endl;
    graph_json.close();
  }

  return 0;
}

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

namespace graph_printing {
std::string vertices_distribution(const Graph& graph) {
  std::string result_string;
  for (int depth = 0; depth < graph.get_depth(); depth++) {
    if (depth != 0)
      result_string += ", ";
    result_string += std::to_string(graph.get_vertices_in_depth_number(depth));
  }
  return result_string;
}

std::string edges_distribution(const Graph& graph) {
  std::string result_string;
  const std::array<Edge::Color, COLORS_NUM> colors = {
      Edge::Color::Gray, Edge::Color::Green, Edge::Color::Yellow,
      Edge::Color::Red};
  for (const auto& color : colors) {
    result_string += uni_cource_cpp::color_to_string(color);
    result_string += ": ";
    result_string += (std::to_string(graph.get_colored_edges(color).size()));
    if (color != Edge::Color::Red)
      result_string += ", ";
  }
  return result_string;
}
}  // namespace graph_printing

std::string generation_started_string(int graph_number) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_number) +
         ", Generation Started\n";
}

std::string generation_finished_string(int graph_number, const Graph& graph) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_number) +
         ", Generation Finished {  \n\tdepth: " +
         std::to_string(graph.get_depth()) + ",\n\tvertices: {amount: " +
         std::to_string(graph.get_vertices_number()) + ", distribution: [" +
         graph_printing::vertices_distribution(graph) +
         "]},\n\tedges: {amount: " + std::to_string(graph.get_edges_number()) +
         ", distribution: {" + graph_printing::edges_distribution(graph) +
         "}}\n}\n\n";
}

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}
