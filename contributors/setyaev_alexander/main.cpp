#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_json_printing.hpp"
#include "graph_printing.hpp"
#include "logger.hpp"

void json_to_file(const std::string& str, const std::string& filepath) {
  std::ofstream json;
  json.open(filepath);
  if (!json.is_open()) {
    throw std::runtime_error("Can't open graph.json file");
  }
  json << str;
  json.close();
}

int handle_depth_input() {
  int depth = 0;
  std::cout << "Please, enter graph's depth:\n";
  std::cin >> depth;
  if (depth < 1) {
    throw std::runtime_error("Incorrect depth! Depth should be above zero!\n");
  }
  return depth;
}

int handle_new_vertices_count_input() {
  int count = 0;
  std::cout << "Please, enter graph's new vertices count:\n";
  std::cin >> count;
  if (count < 1) {
    throw std::runtime_error("Incorrect count! Count should be above zero!\n");
  }
  return count;
}

int handle_graphs_count_input() {
  int count = 0;
  std::cout << "Please, enter graph's count:\n";
  std::cin >> count;
  if (count < 1) {
    throw std::runtime_error("Incorrect count! Count should be above zero!\n");
  }
  return count;
}

const uni_course_cpp::GraphGenerator::Params GraphGenerationParams(
    const int depth,
    const int new_vertices_count) {
  return uni_course_cpp::GraphGenerator::Params(depth, new_vertices_count);
}

void prepare_temp_directory() {
  std::filesystem::create_directory(
      std::string(uni_course_cpp::config::kTempDirectoryPath));
}

std::string generation_started_string(int graph_number) {
  std::stringstream output;
  output << "Graph " << graph_number << ", GenerationStarted";
  return output.str();
}
std::string generation_finished_string(int graph_number,
                                       const std::string& graph_description) {
  std::stringstream output;
  output << "Graph " << graph_number << ", GenerationFinished "
         << graph_description;
  return output.str();
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  auto& logger = uni_course_cpp::Logger::get_logger();
  prepare_temp_directory();

  const auto params = GraphGenerationParams(depth, new_vertices_count);
  const auto generator = uni_course_cpp::GraphGenerator(params);

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate();

    const auto graph_description = uni_course_cpp::printing::print_graph(graph);
    logger.log(generation_finished_string(i, graph_description));

    const auto graph_json =
        uni_course_cpp::printing::json::graph_to_string(graph);
    json_to_file(graph_json, "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
