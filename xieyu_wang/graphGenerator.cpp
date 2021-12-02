#include "graphGenerator.hpp"
GraphGenerator::GraphGenerator(int maxDepth, int newVerticesNum)
    : maxDepth_(maxDepth), newVerticesNum_(newVerticesNum) {}
void GraphGenerator::generateGrey(Graph& graph) const {
  std::vector<int> verticesPresentLevel;
  for (int depth = 0; depth < maxDepth_; depth++) {
    const auto& vertexIds = graph.getVertexIdsAtDepth(depth);
    bool isNewVertexGenerated = false;
    for (const auto& vertexId : vertexIds) {
      for (int i = 0; i < newVerticesNum_; ++i) {
        if (isLucky(getProbabilityGray(depth))) {
          const auto& newVertex = graph.addVertex();
          graph.addEdge(vertexId, newVertex.id, Edge::Color::Gray);
          isNewVertexGenerated = true;
        }
      }
    }
    if (!isNewVertexGenerated) {
      break;
    }
  }
}
void GraphGenerator::generateGreen(Graph& graph) const {
  for (auto& vertex : graph.getVertices()) {
    if (isLucky(GREEN_EDGE_PROBABILITY)) {
      graph.addEdge(vertex.id, vertex.id, Edge::Color::Green);
    }
  }
}
void GraphGenerator::generateRed(Graph& graph) const {
  for (int depth = 0; depth < maxDepth_; depth++) {
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    const auto destinationDepth = graph.getVertexIdsAtDepth(depth + 2);
    for (const auto& fromVertex : presentDepth) {
      for (const auto& toVertex : destinationDepth) {
        if (isLucky(RED_EDGE_PROBABILITY)) {
          graph.addEdge(fromVertex, toVertex, Edge::Color::Red);
        }
      }
    }
  }
}
void GraphGenerator::generateBlue(Graph& graph) const {
  for (int depth = 1; depth < maxDepth_; depth++) {
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    for (auto vertex = presentDepth.begin(); vertex < presentDepth.end();
         vertex++) {
      if (*vertex != presentDepth.back() && isLucky(BLUE_EDGE_PROBABILITY)) {
        graph.addEdge(*vertex, *(vertex + 1), Edge::Color::Blue);
      }
    }
  }
}
void GraphGenerator::generateYellow(Graph& graph) const {
  for (int depth = 0; depth < maxDepth_; depth++) {
    const auto presentLevel = graph.getVertexIdsAtDepth(depth);
    const auto destinationLevel = graph.getVertexIdsAtDepth(depth + 1);
    for (const auto& fromVertex : presentLevel) {
      for (const auto& toVertex : destinationLevel) {
        if (isLucky(getProbabilityYellow(depth)) &&
            !graph.isConnected(fromVertex, toVertex)) {
          graph.addEdge(fromVertex, toVertex, Edge::Color::Yellow);
        }
      }
    }
  }
}

float GraphGenerator::getProbabilityYellow(int depth) const {
  assert(depth >= 0);
  const float result = (float)depth / (float)maxDepth_;
  if (std::isnan(result)) {
    return 0.0;
  }
  return result;
}

Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.addVertex();  // add root vertex
  std::cout << "Generator.................done" << std::endl;
  generateGrey(graph);
  std::cout << "GrayEdgeGenerate..........done" << std::endl;
  generateGreen(graph);
  std::cout << "GreenEdgeGenerate.........done" << std::endl;
  generateBlue(graph);
  std::cout << "BlueEdgeGenerate..........done" << std::endl;
  generateRed(graph);
  std::cout << "redEdgeGenerate...........done" << std::endl;
  generateYellow(graph);
  std::cout << "yellowEdgeGenerate........done" << std::endl;
  return graph;
}

float GraphGenerator::getProbabilityGray(int depth) const {
  assert(depth >= 0);
  const float result = 1.0 - (float)depth / (float)maxDepth_;
  if (std::isnan(result)) {
    return 0.0;
  }
  return result;
}

bool GraphGenerator::isLucky(float probability) const {
  assert(probability >= 0);
  assert(probability <= 1.0);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution distribution(probability);
  return distribution(gen);
}
