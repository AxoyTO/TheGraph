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
    std::cout << "Unable to open file" << std::endl;
  }
}

int main() {
  Graph graph = Graph();
  const int edgesCount = 18;
  const int verticesCount = 14;
  const std::array<GraphInputEdge, edgesCount> inputEdges = {{
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
    EdgeId newEdgeId = graph.addEdge(vertexSrcId, vertexTrgId);
    graph.compliteVertex(vertexSrcId, newEdgeId);
    graph.compliteVertex(vertexTrgId, newEdgeId);
  }

  writeGraphJSON(graph, "graph.json");
  return 0;
}
