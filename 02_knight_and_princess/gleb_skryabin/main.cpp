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
  std::unordered_set<int> inputVertices;
  const std::array<GraphInputEdge, 18> inputEdges = {{
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

  for (const auto& [vertexSrcId, vertexTrgId] : inputEdges) {
    inputVertices.insert(vertexSrcId);
    inputVertices.insert(vertexTrgId);
  }

  for (const int& inputVertex : inputVertices) {
    graph.addVertex(inputVertex);
  }

  for (const auto& [vertexSrcId, vertexTrgId] : inputEdges) {
    EdgeId newEdgeId = graph.addEdge(vertexSrcId, vertexTrgId);
    graph.compliteVertex(vertexSrcId, newEdgeId);
    graph.compliteVertex(vertexTrgId, newEdgeId);
  }

  writeGraphJSON(graph, "graph.json");
  return 0;
}
