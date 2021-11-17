#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "logger.hpp"

using namespace graph_structures;

std::string get_date_and_time() {
  char date_and_time[100] = {0};
  std::time_t now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::strftime(date_and_time, sizeof(date_and_time), "%d.%m.%Y %T",
                std::localtime(&now));
  return date_and_time;
}

int main() {
  int max_depth = 0;
  int new_vertices_num = 0;
  int graphs_count = 0;

  std::cout << "Enter graphs_count: ";
  do {
    std::cin >> graphs_count;
    if (graphs_count < 0)
      std::cerr << "Count of graphs to be created can not be negative!\n"
                   "Enter a non-negative graphs_count: ";
  } while (graphs_count < 0);
  std::cout << "Enter max_depth: ";
  do {
    std::cin >> max_depth;
    if (max_depth < 0)
      std::cerr << "Depth can not be negative!\n"
                   "Enter a non-negative max_depth: ";
  } while (max_depth < 0);
  std::cout << "Enter new_vertices_num: ";
  do {
    std::cin >> new_vertices_num;
    if (new_vertices_num < 0)
      std::cerr << "Number of new vertices created by each vertex can not be "
                   "negative!\nEnter a non-negative new_vertices_num: ";
  } while (new_vertices_num < 0);

  try {
    if (!std::filesystem::create_directory("./temp")) {
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << "\n";
  }

  std::cout << "\n";

  Logger& logger = Logger::get_instance();
  logger.set_file();

  int graph_number = 1;
  for (int i = 0; i < graphs_count; i++) {
    std::string filename = "./temp/graph";
    filename = filename + std::to_string(graph_number) + ".json";
    std::ofstream file(filename, std::ios::out);
    if (!file.is_open())
      std::cerr << "Error opening the file graph " << graph_number << ".json\n";
    else {
      logger.log(get_date_and_time() + ": Graph " +
                 std::to_string(graph_number) + ", Generation Started\n");
      const auto graph = generate_graph(max_depth, new_vertices_num);
      std::vector<std::pair<std::string, int>> colors = {
          {"gray", graph.total_edges_of_color(Edge::Color::Gray)},
          {"green", graph.total_edges_of_color(Edge::Color::Green)},
          {"blue", graph.total_edges_of_color(Edge::Color::Blue)},
          {"yellow", graph.total_edges_of_color(Edge::Color::Yellow)},
          {"red", graph.total_edges_of_color(Edge::Color::Red)}};
      logger.log(get_date_and_time() + ": Graph " +
                 std::to_string(graph_number++) +
                 ", Generation Finished {  \n");
      logger.log("  depth: " + std::to_string(graph.depth()) + ",\n");
      logger.log("  vertices: " + std::to_string(graph.get_vertices().size()) +
                 ", [");
      for (int j = 0; j <= graph.depth(); j++) {
        logger.log(std::to_string(graph.get_vertices_in_depth(j).size()));
        if (j != graph.depth())
          logger.log(", ");
      }
      logger.log("],\n");
      logger.log("  edges: " + std::to_string(graph.get_edges().size()) +
                 ", {");
      for (int j = 0; j < colors.size(); j++) {
        logger.log(colors[j].first + ": " + std::to_string(colors[j].second));
        if (j + 1 != colors.size())
          logger.log(", ");
      }
      logger.log("}\n}\n");
      file << graph.to_JSON();
      file.close();
    }
  }
  return 0;
}
