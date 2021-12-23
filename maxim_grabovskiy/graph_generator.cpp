#include "graph_generator.hpp"
namespace uni_course_cpp {
Graph GraphGenerator::generate() const {
  auto graph = Graph();
  graph.addVertex();
  generateGrayEdges(graph);
  generateGreenEdges(graph);
  generateYellowEdges(graph);
  generateRedEdges(graph);
  return graph;
}
bool GraphGenerator::checkProbability(float probability) const {
  std::random_device seed{};
  std::default_random_engine generator(seed());
  std::bernoulli_distribution generationDistribution(probability);
  return generationDistribution(generator);
}
int GraphGenerator::randomIntNumber(float maximum) const {
  std::random_device seed{};
  std::default_random_engine generator(seed());
  std::uniform_int_distribution<> generationDistribution(0, maximum);
  return generationDistribution(generator);
}

std::vector<VertexId> GraphGenerator::getUnconnectedVertexIds(
    VertexId vertexId,
    const std::vector<VertexId>& vertexIds,
    Graph const& graph) const {
  vector<VertexId> notConnected;
  for (auto randomNextVertexId : vertexIds)
    if (!graph.isConnected(vertexId, randomNextVertexId))
      notConnected.emplace_back(randomNextVertexId);
  return notConnected;
};

void GraphGenerator::generateGrayEdges(Graph& graph) const {
  float const step = 100.0 / (params_.depth - 1);
  for (int currentDepth = 0; currentDepth < params_.depth; currentDepth++) {
    bool generated = false;
    const float generationProb = ((100.0 - (step * (currentDepth))) / 100);
    for (auto parentId : graph.getVertexIdByDepth(currentDepth)) {
      for (int j = 0; j < params_.new_vertices_num; j++) {
        if (checkProbability(generationProb)) {
          graph.spawnVertex(parentId);
          generated = true;
        }
      }
    }
    if (!generated)
      break;
  }
}
void GraphGenerator::generateGreenEdges(Graph& graph) const {
  for (Graph::Vertex const& vertex : graph.getVertexes()) {
    if (checkProbability(GREEN_GENERATION_PROBABILITY))
      graph.addEdge(vertex.id, vertex.id);
  }
}
void GraphGenerator::generateYellowEdges(Graph& graph) const {
  float const step = 100.0 / (params_.depth - 1);
  for (auto vertex : graph.getVertexes()) {
    if (checkProbability(
            ((100.0 - (100.0 - (step * (graph.getDepth(vertex.id))))) / 100)) &&
        graph.getDepth(vertex.id) < params_.depth - 1) {
      auto const NextVertexesIds = getUnconnectedVertexIds(
          vertex.id, graph.getVertexIdByDepth(graph.getDepth(vertex.id) + 1),
          graph);
      if (NextVertexesIds.size() > 0)
        graph.addEdge(
            vertex.id,
            NextVertexesIds[randomIntNumber(NextVertexesIds.size() - 1)]);
    }
  }
}
void GraphGenerator::generateRedEdges(Graph& graph) const {
  for (auto vertex : graph.getVertexes()) {
    if (graph.getDepth(vertex.id) < params_.depth - 2 &&
        checkProbability(RED_GENERATION_PROBABILITY)) {
      auto const NextVertexesIds = getUnconnectedVertexIds(
          vertex.id, graph.getVertexIdByDepth(graph.getDepth(vertex.id) + 2),
          graph);
      if (NextVertexesIds.size() > 0)
        graph.addEdge(
            vertex.id,
            NextVertexesIds[randomIntNumber(NextVertexesIds.size() - 1)]);
    }
  }
}
}  // namespace uni_course_cpp
