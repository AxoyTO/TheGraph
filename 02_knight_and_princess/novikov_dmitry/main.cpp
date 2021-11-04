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
  std::cout << "Enter the Depth of the graph and the Number of new generated "
               "vertices from each vertex of the graph"
            << std::endl;
  std::cin >> depth >> new_vertices_num;
  std::stringstream ss_filename;
  ss_filename << dafault_filename << dafault_fileformat;
  graph_generation::generate_graph(depth, new_vertices_num, ss_filename.str());

  return 0;
}
