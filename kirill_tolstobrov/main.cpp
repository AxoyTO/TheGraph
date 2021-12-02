#include <algorithm>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <sstream>
#include <vector>

#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

std::string color_to_string(uni_cpp_practice::Edge::Color color) {
  switch (color) {
    case uni_cpp_practice::Edge::Color::Grey:
      return "grey";
    case uni_cpp_practice::Edge::Color::Green:
      return "green";
    case uni_cpp_practice::Edge::Color::Blue:
      return "blue";
    case uni_cpp_practice::Edge::Color::Yellow:
      return "yellow";
    case uni_cpp_practice::Edge::Color::Red:
      return "red";
  }
  throw std::runtime_error("Nonexistent color");
}

std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}

const std::string logger_start_string(int graph_number) {
  std::stringstream string;
  string << get_current_date_time() << ": Graph " << graph_number
         << ", Generation Started\n";
  return string.str();
}

const std::string logger_finish_string(int graph_number,
                                       const uni_cpp_practice::Graph& graph) {
  std::stringstream string;
  string << get_current_date_time() << ": Graph " << graph_number
         << ", Generation Finished {\n";
  const auto& depths_map = graph.depths_map_;
  string << " depth: " << depths_map.size() - 1 << ",\n";

  std::stringstream vertices_string;
  vertices_string << "[";
  for (int i = 0; i < depths_map.size(); i++) {
    vertices_string << depths_map[i].size();
    if (i != depths_map.size() - 1) {
      vertices_string << ", ";
    }
  }
  vertices_string << "]";
  string << " vertices: " << graph.get_vertices().size() << ", "
         << vertices_string.str() << ",\n";

  std::stringstream edges_string;
  edges_string << "{";
  const auto& colors = graph.get_colors_map();
  for (auto it = colors.begin(); it != colors.end(); it++) {
    if (it != colors.begin()) {
      edges_string << ", ";
    }
    edges_string << color_to_string(it->first) << ": " << (it->second).size();
  }
  edges_string << "}";
  string << " edges: " << graph.get_edges().size() << ", " << edges_string.str()
         << ",\n";

  string << "}\n\n";

  return string.str();
}

int main() {
  int depth = 0, new_vertices_num = 0, graphs_count = 0;

  std::cout << "Enter the depth:" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Depth can't be negative" << std::endl;
    std::cout << "Enter the depth:" << std::endl;
    std::cin >> depth;
  }

  std::cout << "Enter max amount of vertices genereted from one vertex:"
            << std::endl;
  std::cin >> new_vertices_num;
  while (new_vertices_num < 0) {
    std::cout
        << "Max amount of vertices genereted from one vertex can't be negative"
        << std::endl;
    std::cout << "Enter max amount of vertices genereted from one vertex:"
              << std::endl;
    std::cin >> new_vertices_num;
  }

  std::cout << "Enter number of graphs:" << std::endl;
  std::cin >> graphs_count;
  while (graphs_count < 0) {
    std::cout << "Number of graphs can't be negative" << std::endl;
    std::cout << "Enter number of graphs:" << std::endl;
    std::cin >> graphs_count;
  }

  const uni_cpp_practice::GraphGenerator::Params params(depth,
                                                        new_vertices_num);
  const uni_cpp_practice::GraphGenerator generator(params);

  std::filesystem::create_directory("temp");
  auto& logger = uni_cpp_practice::Logger::get_instance();
  logger.set_file("temp/log.txt");

  for (int i = 0; i < graphs_count; i++) {
    logger.log(logger_start_string(i + 1));

    const uni_cpp_practice::Graph graph = generator.generate_random_graph();

    logger.log(logger_finish_string(i + 1, graph));

    const uni_cpp_practice::GraphPrinter graph_printer(graph);

    std::string filename = "temp/graph_" + std::to_string(i + 1) + ".json";

    std::ofstream file;
    file.open(filename);

    if (file.is_open()) {
      file << graph_printer.print() << std::endl;
      file.close();
    }
  }

  return 0;
}
