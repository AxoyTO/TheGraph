#include "graph_generate.hpp"

int main() {
  const int vertices_num = 14;
  const int edges_num = 18;

  //информация о парах вершин, соединённых рёбрами
  const std::vector<std::pair<int, int>> vertex_pairs = {
      {0, 1},  {0, 2},  {0, 3},  {1, 4},   {1, 5},   {1, 6},
      {2, 7},  {2, 8},  {3, 9},  {4, 10},  {5, 10},  {6, 10},
      {7, 11}, {8, 11}, {9, 12}, {10, 13}, {11, 13}, {12, 13}};

  Graph graph;

  for (int i = 0; i < vertices_num; i++) {
    graph.add_vertex();
  }

  for (auto cur_pair : vertex_pairs) {
    //заполняем номера рёбер для вершин графа
    graph.add_vertex_inf(cur_pair.first);
    graph.add_vertex_inf(cur_pair.second);
    //заполняет номера вершин графа для рёбер
    graph.add_edge(cur_pair.first, cur_pair.second);
  }

  std::ofstream graph_json("graph_json");
  graph_json << graph.json_string() << std::endl;
  graph_json.close();

  return 0;
}
