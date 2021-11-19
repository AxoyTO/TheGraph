#include <array>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "graph.hpp"

void writeGraphJSON(const Graph& graph, const std::string& filename) {
  std::ofstream json(filename);
  if (json.is_open()) {
    json << graph.toJSON();
    json.close();
    std::cout << "Graph is written to " << filename << std::endl;
  } else {
    std::runtime_error("Unable to open file");
  }
}

int main() {
  Graph graph = Graph();
  const int edgesCount = 18;
  const int verticesCount = 14;
  const std::array<std::pair<int, int>, edgesCount> inputEdges = {{
      {0, 1},
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
      {12, 13},
  }};

  for (int i = 0; i < verticesCount; i++) {
    graph.addVertex();
  }

  for (const auto& [vertexSrcId, vertexTrgId] : inputEdges) {
    graph.addEdge(vertexSrcId, vertexTrgId);
  }

  writeGraphJSON(graph, "graph.json");
  return 0;
}
