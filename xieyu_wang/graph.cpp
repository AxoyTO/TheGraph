#include "graph.h"

Graph::Graph() {}
Vertex Graph::addVertex() {
  vertices_.emplace_back(getNextVertexId());
  return vertices_.back();
}
void Graph::addEdge(int fromVertexId, int toVertexId, Edge::Color color) {
  assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
  assert(hasVertex(toVertexId) && "Vertex doesn't exist");
  assert(!isConnected(fromVertexId, toVertexId) &&
         "Vertices already connected");
  const auto& newEdge =
      edges_.emplace_back(getNextEdgeId(), fromVertexId, toVertexId, color);
  if (color != Edge::Color::Green) {
    getVertex(fromVertexId).addEdgeId(newEdge.id);
    getVertex(toVertexId).addEdgeId(newEdge.id);
  } else {
    getVertex(fromVertexId).addEdgeId(newEdge.id);
  }
  if (color == Edge::Color::Gray) {
    getVertex(toVertexId).depth = getVertex(fromVertexId).depth + 1;
  }
}
std::string Graph::toString() const {
  // vertex
  std::string strGraph = "";
  strGraph += "{\n\t\"vertices\": [\n";
  for (const auto& vertex : vertices_) {
    strGraph += vertex.toString() + ",\n";
  }
  strGraph.pop_back();
  strGraph.pop_back();
  strGraph += "\n\t],\n";
  // edges
  strGraph += "\t\"edges\": [\n";
  for (const auto& edge : edges_) {
    strGraph += edge.toString() + ",\n";
  }
  strGraph.pop_back();
  strGraph.pop_back();
  strGraph += "\n\t]\n}\n";
  return strGraph;
}
bool Graph::hasVertex(int idFind) {
  for (const auto& vertex : vertices_) {
    if (vertex.id == idFind) {
      return true;
    }
  }
  return false;
}
bool Graph::isConnected(int fromVertexId, int toVertexId) {
  assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
  assert(hasVertex(toVertexId) && "Vertex doesn't exist");
  if (fromVertexId == toVertexId) {
    return false;
  }
  for (const auto& fromVertexEdgeId : getVertex(fromVertexId).getEdgeIds()) {
    for (const auto& toVertexEdgeId : getVertex(toVertexId).getEdgeIds()) {
      if (fromVertexEdgeId == toVertexEdgeId) {
        return true;
      }
    }
  }
  return false;
}

Vertex& Graph::getVertex(int id) {
  assert(hasVertex(id) && "Vertex doesn't exist");
  for (auto& vertex : vertices_) {
    if (vertex.id == id) {
      return vertex;
    }
  }
  throw std::runtime_error("Unreachable code");
}

int Graph::getPresentVertexId() {
  return edgeIdCounter_;
}

std::vector<int> Graph::getVertexIdsAtDepth(int depth) {
  std::vector<int> vertices;
  for (const auto& vertex : vertices_) {
    if (vertex.depth == depth) {
      vertices.push_back(vertex.id);
    }
  }
  return vertices;
}
