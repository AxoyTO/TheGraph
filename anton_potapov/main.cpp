#include <filesystem>
#include <fstream>
#include <string>

#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_input_handler.hpp"
#include "graph_printer.hpp"
#include "graph_traversal_controller.hpp"
#include "graph_traverser.hpp"
#include "log_messages_generator.hpp"
#include "logger.hpp"

using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerationController;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::GraphInputHandler;
using uni_cource_cpp::GraphPath;
using uni_cource_cpp::GraphPrinter;
using uni_cource_cpp::GraphTraversalController;
using uni_cource_cpp::Logger;
using uni_cource_cpp::LogMessagesGenerator;

void prepare_temp_directory() {
  std::filesystem::create_directory(
      uni_cource_cpp::config::TEMP_DIRECTORY_PATH);
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

std::map<int, Graph> generate_graphs(Logger& logger,
                                     const GraphGenerator::Params& params,
                                     int graphs_count,
                                     int threads_count) {
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);

  auto graphs = std::map<int, Graph>();

  generation_controller.generate(
      [&logger](int index) {
        logger.log(LogMessagesGenerator::generation_started_string(index));
      },
      [&logger, &graphs, &params](int index, Graph graph) {
        const auto graph_printer = GraphPrinter(graph);
        logger.log(LogMessagesGenerator::generation_finished_string(
            index, graph_printer.print_graph_description()));
        if (graph.depth() < params.depth) {
          logger.log(LogMessagesGenerator::generation_max_depth_warning(
              index, graph.depth(), params.depth));
        }
        const auto graph_json = graph_printer.print();
        graphs.emplace(index, std::move(graph));
        write_to_file(graph_json,
                      std::string(uni_cource_cpp::config::TEMP_DIRECTORY_PATH) +
                          "graph_" + std::to_string(index) + ".json");
      });

  return graphs;
}

void traverse_graphs(Logger& logger,
                     std::map<int, Graph>& graphs,
                     int threads_count) {
  auto traversal_controller = GraphTraversalController(threads_count, graphs);

  traversal_controller.traverse(
      [&logger](int index, const Graph& graph) {
        logger.log(LogMessagesGenerator::traversal_started_string(index));
      },
      [&logger](int index, const Graph& graph, std::vector<GraphPath> paths) {
        logger.log(LogMessagesGenerator::traversal_finished_string(
            index, GraphPrinter::print_paths(paths)));
      });
}

int main() {
  const auto depth = GraphInputHandler::handle_depth_input();
  const auto new_vertices_count =
      GraphInputHandler::handle_new_vertices_count_input();
  const auto graphs_count = GraphInputHandler::handle_graphs_count_input();
  const auto threads_count = GraphInputHandler::handle_threads_count_input();

  prepare_temp_directory();

  auto& logger = Logger::get_logger();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  auto graphs = generate_graphs(logger, params, graphs_count, threads_count);

  traverse_graphs(logger, graphs, threads_count);

  return 0;
}
