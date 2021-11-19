#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "graph.hpp"
#include "graph_generator.hpp"
#include "graph_printer.hpp"
#include "logger.hpp"

constexpr int GRAPH_NUMBER = 10;
constexpr int INVALID_NEW_DEPTH = -1;
constexpr int INVALID_NEW_VERTICES_NUM = -1;
const std::string LOG_FILENAME = "temp/log.txt";

int main() {
  std::ofstream log_stream;
  log_stream.open(LOG_FILENAME, std::ofstream::out | std::ofstream::trunc);

  const auto& logger = Logger::get_logger(log_stream);

  int depth = INVALID_NEW_DEPTH;

  do {
    std::cout << "Enter generate graph depth from zero" << std::endl;
    std::cin >> depth;
  } while (depth <= INVALID_NEW_DEPTH);

  int new_vertices_num = INVALID_NEW_VERTICES_NUM;

  do {
    std::cout << "Enter new_vertices_num from zero" << std::endl;
    std::cin >> new_vertices_num;
  } while (new_vertices_num < INVALID_NEW_VERTICES_NUM);

  std::cout << "Depth of adding vertices: " << depth << std::endl;

  for (int graph_num = 0; graph_num < GRAPH_NUMBER; graph_num++) {
    Graph my_graph;
    my_graph.add_vertex();
    new_vertices_generation(my_graph, depth, new_vertices_num);
    paint_edges(my_graph);
    write_graph(my_graph, graph_num);
    write_log(my_graph, log_stream, depth, new_vertices_num, graph_num, logger);
  }

  log_stream.close();

  return 0;
}
