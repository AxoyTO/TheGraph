#include <iostream>
#include <string>
#include "graph.hpp"
#include "graph_generator.hpp"
#include "logger.hpp"

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

  std::cout << "\n";

  Logger log;
  int graph_number = 1;
  for (int i = 0; i < graphs_count; i++) {
    string filename = "./temp/graph";
    filename = filename + std::to_string(graph_number) + ".json";
    std::ofstream file(filename, std::ios::out);
    if (!file.is_open())
      std::cerr << "Error opening the file graph.json!\n";
    else {
      const auto graph = generate_graph(max_depth, new_vertices_num);
      log.start();
      file << graph.to_JSON();
      file.close();
      log.end(graph, graph.depth(), new_vertices_num);
    }
  }
  return 0;
}
