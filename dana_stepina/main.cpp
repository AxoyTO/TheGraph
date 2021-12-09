#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::GraphPrinter;
using uni_cource_cpp::Logger;

using Color = Graph::Edge::Color;
using Params = GraphGenerator::Params;

const std::string FILENAME_FOLDER_PATH = "./temp/";
const std::string FILENAME_PREFIX = "graph_";
const std::string FILENAME_FILE_EXTENSION_JSON = ".json";
const std::string FILENAME_LOG = "log.txt";

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

Logger& prepare_logger() {
  auto& logger = Logger::get_logger();
  logger.set_file(FILENAME_FOLDER_PATH + FILENAME_LOG);
  return logger;
}

std::string generation_started_string(int graph_num) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_num + 1) +
         ", Generation Started";
}

std::string generation_finished_string(int graph_num, const Graph& graph) {
  return get_current_date_time() + ": Graph " + std::to_string(graph_num + 1) +
         ", Generation Finished {  \n" +
         GraphPrinter::print_graph_description(graph);
}

int handle_depth_input() {
  int entered_depth = -1;

  while (entered_depth < 0) {
    std::cout << "Enter depth value: ";
    std::cin >> entered_depth;
    if (entered_depth < 0)
      std::cout << "\nIncorrect depth: " << entered_depth
                << "\nDepth cannot be negative.\n";
    std::cout << std::endl;
  }

  return entered_depth;
}

int handle_new_vertices_num_input() {
  int entered_new_vertices_num = -1;

  while (entered_new_vertices_num < 0) {
    std::cout << "Enter the number of new vertices: ";
    std::cin >> entered_new_vertices_num;
    if (entered_new_vertices_num < 0)
      std::cout
          << "\nIncorrect value of the number of new vertices: "
          << entered_new_vertices_num
          << "\nThe value of the number of new vertices cannot be negative.\n";
    std::cout << std::endl;
  }

  return entered_new_vertices_num;
}

int handle_graphs_count_input() {
  int entered_graphs_count = -1;

  while (entered_graphs_count < 0) {
    std::cout << "Enter the number of new graphs: ";
    std::cin >> entered_graphs_count;
    if (entered_graphs_count < 0)
      std::cout
          << "\nIncorrect value of the number of new graphs: "
          << entered_graphs_count
          << "\nThe value of the number of new graphs cannot be negative.\n";
    std::cout << std::endl;
  }

  return entered_graphs_count;
}

void write_to_file(const std::string& graph_string, const std::string& file) {
  std::ofstream out(file);
  out << graph_string;
  out.close();
}

void prepare_temp_directory() {
  std::filesystem::create_directory(FILENAME_FOLDER_PATH);
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  const auto params = Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  auto& logger = prepare_logger();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate();
    logger.log(generation_finished_string(i, graph));

    const auto graph_printer = GraphPrinter(graph);
    write_to_file(graph_printer.print(),
                  FILENAME_FOLDER_PATH + FILENAME_PREFIX + std::to_string(i) +
                      FILENAME_FILE_EXTENSION_JSON);
  }

  return 0;
}
