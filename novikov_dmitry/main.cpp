#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

const std::string temp_folder_path = "./temp";
const std::string filename_prefix = "Graph";
const std::string filename_suffix = ".json";

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::vector<std::pair<uni_cpp_practice::Edge::Color, int>>
set_count_edges_of_color(const uni_cpp_practice::Graph& graph) {
  std::vector<std::pair<uni_cpp_practice::Edge::Color, int>> colors = {
      {uni_cpp_practice::Edge::Color::Gray,
       graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Gray)},
      {uni_cpp_practice::Edge::Color::Green,
       graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Green)},
      {uni_cpp_practice::Edge::Color::Blue,
       graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Blue)},
      {uni_cpp_practice::Edge::Color::Yellow,
       graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Yellow)},
      {uni_cpp_practice::Edge::Color::Red,
       graph.count_edges_of_color(uni_cpp_practice::Edge::Color::Red)}};
  return colors;
}

std::string gen_started_string(int graph_numbe) {
  std::stringstream log_string;
  log_string << get_current_date_time() << ": Graph " << graph_numbe + 1
             << ", Generation Started\n";
  return log_string.str();
}

std::string gen_finished_string(int graph_numbe,
                                const uni_cpp_practice::Graph& graph) {
  std::stringstream log_string;
  log_string << get_current_date_time() << ": Graph " << graph_numbe + 1
             << ", Generation Finished {  \n";
  log_string << "  depth: " << graph.get_depth() << ",\n";
  log_string << "  vertices: " << graph.get_vertex_map().size() << ", [";
  for (uni_cpp_practice::Depth current_depth = 0;
       current_depth <= graph.get_depth(); ++current_depth) {
    log_string << graph.get_vertices_at_depth(current_depth).size();
    if (current_depth != graph.get_depth()) {
      log_string << ", ";
    }
  }
  log_string << "],\n";
  log_string << "  edges: " << graph.get_edge_map().size() << ", {";
  const auto& count_edges_of_color_map = set_count_edges_of_color(graph);
  for (int color_index = 0; color_index < count_edges_of_color_map.size();
       ++color_index) {
    log_string << uni_cpp_practice::color_to_string(
                      count_edges_of_color_map[color_index].first)
               << ": " << count_edges_of_color_map[color_index].second;
    if (color_index + 1 != count_edges_of_color_map.size()) {
      log_string << ", ";
    }
  }
  log_string << "}\n}\n";
  return log_string.str();
}

uni_cpp_practice::Depth handle_depth_input() {
  uni_cpp_practice::Depth
      depth;  //Глубина графов (int от 0 и до бесконечности).
  std::cout << "Enter the Depth of the graphs" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> depth;
  }
  return depth;
}
int handle_new_vertices_num_input() {
  int new_vertices_num;  //Количество вершин, генерируемых каждой родительской
                         //вершиной.
  std::cout << "Enter the Number of new vertices from each vertex of the graphs"
            << std::endl;
  std::cin >> new_vertices_num;
  while (new_vertices_num < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> new_vertices_num;
  }
  return new_vertices_num;
}

int handle_graphs_count_input() {
  int graphs_count;  // Количество генерируемых графов.
  std::cout << "Enter the Count of new graphs" << std::endl;
  std::cin >> graphs_count;
  while (graphs_count <= 0) {
    std::cout << "Try to enter a natural number" << std::endl;
    std::cin >> graphs_count;
  }
  return graphs_count;
}
int handle_threads_count_input() {
  int threads_count;  // Количество потоков генерации графов.
  std::cout << "Enter the Count of threads for graphs generation" << std::endl;
  std::cin >> threads_count;
  while (threads_count <= 0) {
    std::cout << "Try to enter a natural number" << std::endl;
    std::cin >> threads_count;
  }
  return threads_count;
}
uni_cpp_practice::Logger& prepare_logger() {
  try {
    std::filesystem::create_directory("./temp");
  } catch (const std::exception& ex) {
    std::cout << ex.what() << "\n";
  }
  auto& logger = uni_cpp_practice::Logger::get_instance();
  logger.set_file("./temp/log.txt");
  return logger;
}

void write_to_file(const uni_cpp_practice::GraphPrinter& graph_printer,
                   const std::string& filename) {
  std::ofstream file_out;
  file_out.open(filename, std::fstream::out | std::fstream::trunc);
  if (!file_out.is_open()) {
    std::cerr << "Error opening the file " << filename;
  } else {
    file_out << graph_printer.print();
    file_out.close();
  }
}

using uni_cpp_practice::Graph;
using uni_cpp_practice::GraphGenerationController;
using uni_cpp_practice::GraphGenerator;
using uni_cpp_practice::GraphPrinter;
int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();

  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);
  auto& logger = prepare_logger();

  auto graphs = std::vector<Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(gen_started_string(index)); },
      [&logger, &graphs](int index, Graph graph) {
        logger.log(gen_finished_string(index, graph));
        graphs.push_back(graph);
        const auto graph_printer = GraphPrinter(graph);
        write_to_file(graph_printer, temp_folder_path + '/' + filename_prefix +
                                         "_" + std::to_string(index) +
                                         filename_suffix);
      });

  return 0;
}
