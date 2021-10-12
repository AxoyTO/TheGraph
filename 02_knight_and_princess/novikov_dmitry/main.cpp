#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Graph_generator.hpp"

void graphs_generation() {
  const std::string DEFAULT_FILENAME = "Graph";
  const std::string DEFAULT_FILEFORMAT = ".json";

  Depth depth;  //Глубина графа (int от 0 и до бесконечности).
  int new_vertices_num;  //Количество новых генерируемых вершин из каждой
                         //вершины графа (int от 0 и до бесконечности).
  int graph_num;
  std::cout << "Enter the Depth of the graph and the Number of new generated "
               "vertices from each vertex of the graph"
            << std::endl;
  std::cin >> depth >> new_vertices_num;
  std::cout << "Enter the number of graphs to generate" << std::endl;
  std::cin >> graph_num;  //количество графов для генерации

  for (int i = 0; i < graph_num; ++i) {
    Graph g = Graph();
    graph_generation(g, depth, new_vertices_num);
    std::stringstream ss_filename;
    ss_filename << DEFAULT_FILENAME << '_' << i << DEFAULT_FILEFORMAT;
    std::ofstream file_out;
    file_out.open(ss_filename.str(), std::fstream::out | std::fstream::trunc);
    file_out << g.to_string();
    file_out.close();
  }
}

int main() {
  graphs_generation();
  return 0;
}
