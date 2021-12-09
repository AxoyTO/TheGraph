#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "config.hpp"
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using uni_course_cpp::Graph;
using uni_course_cpp::GraphGenerator;
using uni_course_cpp::Logger;

constexpr int MIN_DEPTH = 0;
constexpr int MIN_NEW_VERTICES_NUM = 0;
constexpr int MIN_GRAPHS_COUNT = 0;

const std::string FILENAME_PREFIX = "graph_";
const std::string FILENAME_SUFFIX = ".json";

const int handle_depth_input() {
  int depth;

  std::cout << "Input depth:" << std::endl;
  while (!(std::cin >> depth) || (depth < MIN_DEPTH)) {
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return depth;
}

const int handle_new_vertices_num_input() {
  int vertices_num;
  std::cout << "Input vertices num:" << std::endl;
  while (!(std::cin >> vertices_num) || (vertices_num < MIN_NEW_VERTICES_NUM)) {
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return vertices_num;
}

const int handle_graphs_count_input() {
  int graphs_count;
  std::cout << "Input graphs count:" << std::endl;
  while (!(std::cin >> graphs_count) || (graphs_count < MIN_GRAPHS_COUNT)) {
    std::cout << "Invalid value, please input natural number or zero:"
              << std::endl;
    if (!std::cin) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return graphs_count;
}

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

std::string gen_started_string(int graph_number) {
  std::stringstream res;
  res << get_current_date_time() << ": Graph " << graph_number + 1
      << ", Generation Started\n";
  return res.str();
}

std::string gen_finished_string(int graph_number, const Graph& graph) {
  std::stringstream res;
  res << get_current_date_time() << ": Graph " << graph_number + 1
      << ", Generation Finished ";

  res << uni_course_cpp::graph_printing::print_graph_description(graph);

  res << "}\n}\n";
  return res.str();
}

void prepare_temp_directory() {
  std::filesystem::create_directory(
      uni_course_cpp::config::TEMP_DIRECTORY_PATH);
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_num = handle_new_vertices_num_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);

  auto& logger = Logger::get_instance();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(gen_started_string(i));
    const auto graph = generator.generate();
    logger.log(gen_finished_string(i, graph));

    std::ofstream myfile;
    myfile.open(uni_course_cpp::config::TEMP_DIRECTORY_PATH + FILENAME_PREFIX +
                std::to_string(i) + FILENAME_SUFFIX);
    myfile << uni_course_cpp::graph_printing::print_graph(graph);
    myfile.close();
  }

  return 0;
}
