#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "config.hpp"
#include "graph.hpp"
#include "graph_generation_controller.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

using std::cin;
using std::cout;
using std::string;
using std::to_string;
using uni_cource_cpp::Graph;
using uni_cource_cpp::GraphGenerationController;
using uni_cource_cpp::GraphGenerator;
using uni_cource_cpp::Logger;
const int kMaxAvailableThreads = std::thread::hardware_concurrency();
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
                                  const string& graph_description) {
  string finished_string = get_current_date_time() + ": Graph " +
                           to_string(graph_number) +
                           ", Generation Finished {\n";

  finished_string = finished_string + graph_description + "\n}";

  return finished_string;
}

void write_to_file(const string& json, const string& file_name) {
  std::ofstream file;
  file.open(file_name);
  file << json;
}

std::vector<Graph> generate_graphs(const GraphGenerator::Params& params,
                                   int graphs_count,
                                   int threads_count) {
  auto generation_controller =
      GraphGenerationController(threads_count, graphs_count, params);

  auto& logger = Logger::get_logger();

  auto graphs = std::vector<Graph>();
  graphs.reserve(graphs_count);

  generation_controller.generate(
      [&logger](int index) { logger.log(generation_started_string(index)); },
      [&logger, &graphs](int index, Graph graph) {
        const auto graph_description =
            uni_cource_cpp::graph_printing::print_graph_description(graph);
        logger.log(generation_finished_string(index, graph_description));
        graphs.push_back(graph);
        const auto graph_json =
            uni_cource_cpp::graph_printing::print_graph(graph);
        write_to_file(graph_json,
                      string(uni_cource_cpp::config::kTempDirectoryPath) +
                          "graph_" + to_string(index) + ".json");
      });

  return graphs;
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

int handle_threads_count_input() {
  int threads_count;
  while (true) {
    cout << "Input amount of threads:\n";
    cin >> threads_count;
    if (threads_count < 0) {
      cout << "Incorrect input. Number of threads must be integer and >= 0\n";
    }
    if (threads_count > kMaxAvailableThreads) {
      cout << "Incorrect input. Such amount of threads is not available\n";
    } else {
      break;
    }
  }
  return threads_count;
}

void prepare_temp_directory() {
  std::filesystem::create_directory(uni_cource_cpp::config::kTempDirectoryPath);
}

int main() {
  const int depth = handle_depth_input();
  const int new_vertices_count = handle_new_vertices_count_input();
  const int graphs_count = handle_graphs_count_input();
  const int threads_count = handle_threads_count_input();
  prepare_temp_directory();

  const auto params = GraphGenerator::Params(depth, new_vertices_count);
  const auto graphs = generate_graphs(params, graphs_count, threads_count);

  return 0;
}
