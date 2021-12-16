#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using std::cin;
using std::cout;
using std::string;
using std::to_string;

string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

string generation_started_string(int graph_number) {
  return get_current_date_time() + ": Graph " + to_string(graph_number) +
         ", Generation Started";
}

string generation_finished_string(int graph_number,
                                  const uni_cource_cpp::Graph& graph) {
  string finished_string = get_current_date_time() + ": Graph " +
                           to_string(graph_number) +
                           ", Generation Finished {\n";

  finished_string =
      finished_string +
      uni_cource_cpp::graph_printing::print_graph_description(graph) + "\n}";

  return finished_string;
}

void write_to_file(const string& json, const string& file_name) {
  std::ofstream file;
  file.open(file_name);
  file << json;
}

int handle_depth_input() {
  int depth;
  while (true) {
    cout << "Enter depth of graph:\n";
    cin >> depth;
    if (depth < 0) {
      std::cout << "Incorrect input. Depth must be integer and >= 0\n";
      cin.clear();
    } else {
      break;
    }
  }
  return depth;
}

int handle_new_vertices_count_input() {
  int new_vertices_count;
  while (true) {
    cout << "Enter max number of vertices for each vertex:\n";
    cin >> new_vertices_count;
    if (new_vertices_count < 0) {
      cout << "Incorrect input. Number of vertices must be integer and >= 0\n";
    } else {
      break;
    }
  }
  return new_vertices_count;
}

int handle_graphs_count_input() {
  int graphs_count;
  while (true) {
    cout << "Input amount of graphs:\n";
    cin >> graphs_count;
    if (graphs_count < 0) {
      cout << "Incorrect input. Number of graph must be integer and >= 0\n";
    } else {
      break;
    }
  }
  return graphs_count;
}

void prepare_temp_directory() {
  std::filesystem::create_directory("temp/");
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  prepare_temp_directory();

  const auto params =
      uni_cource_cpp::GraphGenerator::Params(depth, new_vertices_count);
  const auto generator = uni_cource_cpp::GraphGenerator(params);
  auto& logger = uni_cource_cpp::Logger::get_logger();
  logger.set_file_path();

  for (int i = 0; i < graphs_count; i++) {
    logger.log(generation_started_string(i));
    const auto graph = generator.generate();
    logger.log(generation_finished_string(i, graph));

    const auto graph_json = uni_cource_cpp::graph_printing::print_graph(graph);
    write_to_file(graph_json, "temp/graph_" + std::to_string(i) + ".json");
  }

  return 0;
}