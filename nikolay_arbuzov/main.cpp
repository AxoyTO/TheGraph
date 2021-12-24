#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "config.hpp"
#include "graph.hpp"
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
void write_to_file(const std::string& graph_json,
                   const std::string& file_path) {
  std::ofstream out_file;
  out_file.open(std::string(config::kTempDirectoryPath) + file_path);
  out_file << graph_json;
  out_file.close();
}
std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string generation_started_string(const int graph_number) {
  std::stringstream log_string;
  log_string << get_current_date_time() << " Graph " << graph_number
             << " Generation Started";
  return log_string.str();
}

std::string generation_finished_string(const int graph_number,
                                       const std::string& graph_description) {
  std::stringstream log_string;
  log_string << get_current_date_time() << " Graph " << graph_number
             << " Generation Finished {" << std::endl;
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

}  // namespace

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  const auto params =
      uni_course_cpp::GraphGenerator::Params(depth, new_vertices_count);
  auto generator = uni_course_cpp::GraphGenerator(params);
  auto& logger = uni_course_cpp::Logger::get_logger();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto& graph = generator.generate();

    const auto& graph_description =
        uni_course_cpp::printing::print_graph(graph);
    logger.log(generation_finished_string(i, graph_description));
    const auto graph_json = uni_course_cpp::printing::json::print_graph(graph);
    write_to_file(graph_json, "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
