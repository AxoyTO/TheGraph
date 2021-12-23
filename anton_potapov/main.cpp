#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_input_handler.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerationController;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::GraphInputHandler;
using uni_cource_cpp::GraphPrinter;
using uni_cource_cpp::Logger;

void prepare_temp_directory() {
  std::filesystem::create_directory(
      uni_cource_cpp::config::TEMP_DIRECTORY_PATH);
}

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string generation_started_string(int id) {
  std::stringstream generation_started_stringstream;
  generation_started_stringstream << get_current_date_time() << ": Graph " << id
                                  << ", Generation Started" << std::endl;
  return generation_started_stringstream.str();
}

std::string generation_finished_string(int id, Graph& graph) {
  std::stringstream generation_finished_stringstream;
  generation_finished_stringstream
      << get_current_date_time() << ": Graph " << id << ", Generation Finished "
      << GraphPrinter(graph).print_graph_description() << std::endl;
  return generation_finished_stringstream.str();
}

std::string generation_max_depth_warning(int id,
                                         int max_generated_depth,
                                         int given_depth) {
  std::stringstream generation_max_depth_warning_stringstream;
  generation_max_depth_warning_stringstream
      << get_current_date_time() << ": Graph " << id
      << "generated graph's depth=" << max_generated_depth
      << " is less than specified one =" << given_depth << std::endl;
  return generation_max_depth_warning_stringstream.str();
}

void write_to_file(const std::string& file_text, const std::string& file_path) {
  std::fstream json_file;
  json_file.open(file_path, std::ios::out);
  if (!json_file.is_open()) {
    throw std::runtime_error("file not created");
  }
  json_file << file_text << std::endl;
  json_file.close();
}

int main() {
  const auto depth = GraphInputHandler::handle_depth_input();
  const auto new_vertices_count =
      GraphInputHandler::handle_new_vertices_count_input();
  const auto graphs_count = GraphInputHandler::handle_graphs_count_input();
  const auto threads_count = GraphInputHandler::handle_threads_count_input();

  prepare_temp_directory();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(params);
  auto& logger = Logger::get_logger();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    auto graph = generator.generate_graph();
    if (graph.depth() < depth) {
      logger.log(generation_max_depth_warning(i, graph.depth(), depth));
    }
    logger.log(generation_finished_string(i, graph));

    const auto graph_printer = GraphPrinter(graph);
    const auto graph_json = graph_printer.print();
    write_to_file(graph_json,
                  std::string(uni_cource_cpp::config::TEMP_DIRECTORY_PATH) +
                      "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
