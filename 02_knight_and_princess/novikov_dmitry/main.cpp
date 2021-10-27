#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "graph_generation.hpp"

void generate_graphs() {
  const std::string DEFAULT_FILENAME = "Graph";
  const std::string DEFAULT_FILEFORMAT = ".json";

  int new_vertices_num;
  Depth depth;  //Глубина графа (int от 0 и до бесконечности).
  std::cout << "Enter the Depth of the graph and the Number of new generated "
               "vertices from each vertex of the graph"
            << std::endl;
  std::cin >> depth >> new_vertices_num;

  const auto& graph = graph_generation::generate_graph(depth, new_vertices_num);
  std::stringstream ss_filename;
  ss_filename << DEFAULT_FILENAME << '_' << DEFAULT_FILEFORMAT;
  std::ofstream file_out;
  file_out.open(ss_filename.str(), std::fstream::out | std::fstream::trunc);
  file_out << graph.to_string();
  file_out.close();
}

int main() {
  generate_graphs();
  return 0;
}
