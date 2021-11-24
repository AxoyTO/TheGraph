#include <fstream>
#include "graph.hpp"

constexpr int VERTICES_NUM = 14;
constexpr int EDGES_NUM = 18;

//информация о парах вершин, соединённых рёбрами
const std::array<std::pair<VertexId, VertexId>, EDGES_NUM> vertices_pairs = {
    {{0, 1},
     {0, 2},
     {0, 3},
     {1, 4},
     {1, 5},
     {1, 6},
     {2, 7},
     {2, 8},
     {3, 9},
     {4, 10},
     {5, 10},
     {6, 10},
     {7, 11},
     {8, 11},
     {9, 12},
     {10, 13},
     {11, 13},
     {12, 13}}};

int main() {
  Graph graph;

  for (int i = 0; i < VERTICES_NUM; i++) {
    graph.add_vertex();
  }

  for (const auto& vertices_pair : vertices_pairs) {
    graph.add_edge(vertices_pair.first, vertices_pair.second);
  }

  std::ofstream graph_json("graph_json");
  graph_json << graph.json_string() << std::endl;
  graph_json.close();

  return 0;
}
