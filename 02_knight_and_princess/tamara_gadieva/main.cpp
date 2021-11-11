#include "graph.hpp"

int main() {
  Graph graph;

  for (int i = 0; i < kvertices_num; i++) {
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
