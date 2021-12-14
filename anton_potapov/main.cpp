#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::GraphPrinter;
using uni_cource_cpp::Logger;

int handle_depth_input() {
  while (true) {
    std::cout << "Input the depth parameter: " << std::flush;
    int depth;
    std::cin >> depth;
    if (depth >= 0) {
      return depth;
    }
    std::cout << "depth should be > 0, please, redo the input:" << std::endl;
  }
}

int handle_new_vertices_count_input() {
  while (true) {
    std::cout << "Input the amount of new vertices: " << std::flush;
    int new_vertices_num;
    std::cin >> new_vertices_num;
    if (new_vertices_num >= 0) {
      return new_vertices_num;
    }
    std::cout << "new vertices num should be > 0, please, redo the input:"
              << std::endl;
  }
}

int handle_graphs_count_input() {
  while (true) {
    std::cout << "Input the amount of graphs to be generated: " << std::flush;
    int graphs_count;
    std::cin >> graphs_count;
    if (graphs_count >= 0) {
      return graphs_count;
    }
    std::cout << "new vertices num should be > 0, please, redo the input:"
              << std::endl;
  }
}

void prepare_temp_directory() {
  const auto& temp_directory_path = config::kTempDirectoryPath;
  if (!std::filesystem::is_directory(temp_directory_path) ||
      !std::filesystem::exists(temp_directory_path)) {
    std::filesystem::create_directory(temp_directory_path);
  }
}

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

namespace graph_printing {
std::string print_graph_description(const Graph& graph) {
  std::stringstream graph_description_stringstream;
  graph_description_stringstream << "{" << std::endl;
  graph_description_stringstream << "\t"
                                 << "depth: " << graph.max_depth() << ","
                                 << std::endl;
  graph_description_stringstream
      << "\t"
      << "vertices: {amount: " << graph.vertices().size()
      << ", distribution: [";
  for (int depth = 0; depth <= graph.max_depth(); ++depth) {
    graph_description_stringstream << graph.get_vertices_at_depth(depth).size();
    if (depth != graph.max_depth()) {
      graph_description_stringstream << ", ";
    }
  }
  graph_description_stringstream << "]}," << std::endl;
  graph_description_stringstream
      << "\t"
      << "edges: {amount: " << graph.edges().size() << ", distribution: {"
      << "gray: " << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Gray)
      << ", green: "
      << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Green)
      << ", blue: "
      << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Blue)
      << ", yellow: "
      << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Yellow)
      << ", red: " << graph.edge_with_color_cnt(uni_cource_cpp::EdgeColor::Red);
  graph_description_stringstream << "}}" << std::endl;
  graph_description_stringstream << "}" << std::endl;
  return graph_description_stringstream.str();
}
}  // namespace graph_printing

std::string generation_started_string(int id) {
  std::stringstream generation_started_stringstream;
  generation_started_stringstream << get_current_date_time() << ": Graph " << id
                                  << ", Generation Started" << std::endl;
  return generation_started_stringstream.str();
}

std::string generation_finished_string(int id, const Graph& graph) {
  std::stringstream generation_finished_stringstream;
  generation_finished_stringstream
      << get_current_date_time() << ": Graph " << id << ", Generation Finished "
      << graph_printing::print_graph_description(graph) << std::endl;
  return generation_finished_stringstream.str();
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
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = GraphGenerator(params);
  auto& logger = Logger::get_logger();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate_graph();
    if (graph.max_depth() < depth) {
      std::cerr << "generated graph's depth=" << graph.max_depth()
                << " is less than specified one =" << depth << std::endl;
    }
    logger.log(generation_finished_string(i, graph));

    auto graph_printer = GraphPrinter(graph);
    const auto graph_json = graph_printer.print();
    write_to_file(graph_json, "graph_" + std::to_string(i) + ".json");
  }

  return 0;
}
