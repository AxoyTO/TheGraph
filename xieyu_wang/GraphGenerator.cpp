#include "GraphGenerator.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
constexpr float GREEN_EDGE_PROBABILITY = 0.10;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;
GraphGenerator::GraphGenerator(int maxDepth, int newVerticesNum)
    : maxDepth_(maxDepth), newVerticesNum_(newVerticesNum) {}
void GraphGenerator::generateGrey(Graph& graph) const {
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
  for (const auto& vertex : graph.getVertices()) {
    if (isLucky(GREEN_EDGE_PROBABILITY)) {
      graph.addEdge(vertex.id, vertex.id, Edge::Color::Green);
    }
  }
}
void GraphGenerator::generateRed(Graph& graph) const {
  for (int depth = 0; depth < maxDepth_ - 1; depth++) {
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    const auto destinationDepth = graph.getVertexIdsAtDepth(depth + 2);
    for (const auto& fromVertexId : presentDepth) {
      if (isLucky(RED_EDGE_PROBABILITY)) {
        if (!destinationDepth.empty()) {
          const auto randomVertexId = getRandomVertexId(destinationDepth);
          graph.addEdge(fromVertexId, randomVertexId, Edge::Color::Red);
        }
      }
    }
  }
}
void GraphGenerator::generateBlue(Graph& graph) const {
  for (int depth = 1; depth < maxDepth_; depth++) {
    const auto presentDepth = graph.getVertexIdsAtDepth(depth);
    for (auto vertexIt = presentDepth.begin();
         vertexIt < presentDepth.end() - 1; vertexIt++) {
      if (isLucky(BLUE_EDGE_PROBABILITY)) {
        graph.addEdge(*vertexIt, *(vertexIt + 1), Edge::Color::Blue);
      }
    }
  }
}
void GraphGenerator::generateYellow(Graph& graph) const {
  for (int depth = 0; depth < maxDepth_; depth++) {
    const auto presentLevel = graph.getVertexIdsAtDepth(depth);
    const auto destinationLevel = graph.getVertexIdsAtDepth(depth + 1);
    for (const auto& fromVertexId : presentLevel) {
      if (isLucky(getProbabilityYellow(depth))) {
        const auto unconnectedVertexIds =
            getUnconnectedVertexIds(fromVertexId, destinationLevel, graph);
        if (!unconnectedVertexIds.empty()) {
          const auto randomVertexId = getRandomVertexId(unconnectedVertexIds);
          graph.addEdge(fromVertexId, randomVertexId, Edge::Color::Yellow);
        }
      }
    }
  }
}

float GraphGenerator::getProbabilityYellow(int depth) const {
  assert(depth >= 0);
  const float result = (float)depth / (float)(maxDepth_ - 1);
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

std::vector<int> GraphGenerator::getUnconnectedVertexIds(
    const int fromVertexId,
    const std::vector<int>& destinationLevel,
    Graph& graph) const {
  std::vector<int> unconnectedVertexIds;
  for (const auto& vertexId : destinationLevel) {
    if (!graph.isConnected(fromVertexId, vertexId)) {
      unconnectedVertexIds.push_back(vertexId);
    }
  }
  return unconnectedVertexIds;
}

bool isLucky(float probability) {
  assert(probability >= 0);
  assert(probability <= 1.0);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::bernoulli_distribution distribution(probability);
  return distribution(gen);
}

int getRandomVertexId(const std::vector<int> destinationLevelIds) {
  assert(!destinationLevelIds.empty() && "destinationLevelIds is empty!!!");
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<std::mt19937::result_type> vertexIndex(
      0, destinationLevelIds.size() - 1);
  return destinationLevelIds[vertexIndex(gen)];
}
