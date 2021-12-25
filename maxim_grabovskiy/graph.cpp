#include "graph.hpp"
#include <stdexcept>

namespace uni_course_cpp {
void Graph::addEdge(VertexId fromVertexId, VertexId toVertexId) {
  EdgeId const newEdgeId = getNewEdgeId();
  Edge::Color color = calculate_color(fromVertexId, toVertexId);

  colorsDistributionMap_[color].emplace_back(newEdgeId);

  auto const& fromVertex = vertexes_[fromVertexId];
  auto const& toVertex = vertexes_[toVertexId];
  edgeConectionMap_[fromVertexId].push_back(newEdgeId);
  if (color != Edge::Color::Green)
    edgeConectionMap_[toVertexId].push_back(newEdgeId);
  if (color == Edge::Color::Gray) {
    for (int i = 0; i < (int)vertexIdByDepth_[0].size() - 1; i++) {
      if (vertexIdByDepth_[0][i] == toVertexId)
        vertexIdByDepth_.erase(vertexIdByDepth_.begin() + i);
    }
    auto const newDepth = getDepth(fromVertex.id) + 1;
    if ((int)vertexIdByDepth_.size() - 1 < newDepth)
      vertexIdByDepth_.emplace_back();

    vertexIdByDepth_[newDepth].emplace_back(toVertexId);
    setDepth(toVertex.id, newDepth);
  }
  edges_.emplace_back(newEdgeId, fromVertexId, toVertexId, color);
}
std::vector<EdgeId> const& Graph::getColorDistribution(
    Edge::Color const& color) const {
  if (colorsDistributionMap_.find(color) == colorsDistributionMap_.end()) {
    static std::vector<EdgeId> const empty_result = {};
    return empty_result;
  }
  return colorsDistributionMap_.at(color);
}

Graph::Edge::Color Graph::calculate_color(VertexId fromVertexId,
                                          VertexId toVertexId) {
  auto const& fromVertex = vertexes_[fromVertexId];
  auto const& toVertex = vertexes_[toVertexId];
  if ((getDepth(fromVertex.id) - getDepth(toVertex.id)) == -1) {
    return Edge::Color::Yellow;
  }
  if ((getDepth(fromVertex.id) - getDepth(toVertex.id)) == -2) {
    return Edge::Color::Red;
  }
  if (toVertexId == fromVertexId) {
    return Edge::Color::Green;
  }
  if ((getDepth(fromVertex.id) - getDepth(toVertex.id)) >= 0) {
    return Edge::Color::Gray;
  }
  throw std::runtime_error("cannot calculate color");
}
VertexId Graph::addVertex() {
  VertexId const newVertexId = getNewVertexId();
  auto const& new_vertex = vertexes_.emplace_back(newVertexId);
  if (!newVertexId)
    vertexIdByDepth_.emplace_back();
  vertexIdByDepth_[0].emplace_back(new_vertex.id);
  setDepth(newVertexId, 0);
  edgeConectionMap_[newVertexId] = {};
  return newVertexId;
}
VertexId Graph::spawnVertex(VertexId parentId) {
  VertexId const newVertexId = addVertex();
  addEdge(parentId, newVertexId);
  return newVertexId;
}
bool Graph::isConnected(VertexId firstVertexId, VertexId secondVertexId) const {
  if (firstVertexId == secondVertexId) {
    for (auto edgeId : getConnections(firstVertexId)) {
      if (getEdge(edgeId).color == Edge::Color::Green)
        return true;
    }
  } else {
    for (auto edgeId : getConnections(firstVertexId)) {
      auto connectedfrom = getEdge(edgeId).fromVertexId;
      auto connectedTo = getEdge(edgeId).toVertexId;
      if (connectedfrom != connectedTo) {
        if (connectedfrom == secondVertexId || connectedTo == secondVertexId)
          return true;
      }
    }
  }
  return false;
};
}  // namespace uni_course_cpp
