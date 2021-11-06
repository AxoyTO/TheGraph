#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "graph_generation.hpp"

int main() {
  const std::string dafault_filename = "Graph";
  const std::string dafault_fileformat = ".json";

  int new_vertices_num;
  Depth depth;  //Глубина графа (int от 0 и до бесконечности).
  std::cout << "Enter the Depth of the graph" << std::endl;
  std::cin >> depth;
  while (depth < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> depth;
  }
  std::cout << "Enter the Number of new vertices from each vertex of the graph"
            << std::endl;
  std::cin >> new_vertices_num;
  while (new_vertices_num < 0) {
    std::cout << "Try to enter a natural number or zero" << std::endl;
    std::cin >> new_vertices_num;
  }
  std::stringstream ss_filename;
  ss_filename << dafault_filename << dafault_fileformat;
  const auto graph = graph_generation::generate_graph(depth, new_vertices_num);
  std::ofstream file_out;
  file_out.open(ss_filename.str(), std::fstream::out | std::fstream::trunc);
  file_out << graph.to_string();
  file_out.close();

  return 0;
}
