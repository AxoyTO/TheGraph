#include <assert.h>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
class Vertex {
 public:
  const int id = 0;
  explicit Vertex(int id) : id(id) {}
  void addEdgeId(int eId) {
    assert(!hasEdgeId(eId) && "Edge Id already exists");
    edgeIds_.push_back(eId);
  }
  std::string toString() const {
    std::string str = "";
    str += "\t\t{\"id\":" + std::to_string(id) + ",";
    str += "\"edge_ids\": [";
    for (const auto& edgeId : edgeIds_) {
      str += std::to_string(edgeId) + ",";
    }
    str.pop_back();
    str += "]}";
    return str;
  }
  bool hasEdgeId(int id) const {
    for (const auto& eId : edgeIds_) {
      if (id == eId) {
        return true;
      }
    }
    return false;
  }
  std::vector<int> getEdgeIds() const { return edgeIds_; }

 private:
  std::vector<int> edgeIds_;
};

class Edge {
 public:
  const int id = 0;
  const int fromVertexId = 0;
  const int toVertexId = 0;
  Edge(int id, int fromVertexId, int toVertexId)
      : id(id), fromVertexId(fromVertexId), toVertexId(toVertexId) {}
  std::string toString() const {
    std::string strEdge = "";
    strEdge += "\t\t{\"id\": " + std::to_string(id) + ",";
    strEdge += "\"vertex_ids\": [" + std::to_string(fromVertexId) + "," +
               std::to_string(toVertexId) + "]}";
    return strEdge;
  }
};
class Graph {
 public:
  void addVertex() { vertices_.emplace_back(getNextVertexId()); }
  void addEdge(int fromVertexId, int toVertexId) {
    assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
    assert(hasVertex(toVertexId) && "Vertex doesn't exist");
    assert(!isConnected(fromVertexId, toVertexId) &&
           "Vertices already connected");
    const auto& newEdge =
        edges_.emplace_back(getNextEdgeId(), fromVertexId, toVertexId);
    for (auto& vertex : vertices_) {
      if (fromVertexId == vertex.id || toVertexId == vertex.id) {
        vertex.addEdgeId(newEdge.id);
      }
    }
  }
  std::string toString() {
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
  bool hasVertex(int idFind) {
    for (const auto& vertex : vertices_) {
      if (vertex.id == idFind) {
        return true;
      }
    }
    return false;
  }
  bool isConnected(int fromVertexId, int toVertexId) {
    if (!hasVertex(fromVertexId) || !hasVertex(toVertexId)) {
      std::cerr << "Oh man Vertex " << fromVertexId << " or " << toVertexId
                << " not exists.";
      std::exit(-1);
    }
    std::vector<int> fromVertexEdgeIds;
    std::vector<int> toVertexEdgeIds;
    std::vector<int> intersections;
    for (const auto& vertex : vertices_) {
      if (vertex.id == fromVertexId) {
        fromVertexEdgeIds = vertex.getEdgeIds();
      }
      if (vertex.id == toVertexId) {
        toVertexEdgeIds = vertex.getEdgeIds();
      }
    }
    for (const auto& fromVertexEdgeId : fromVertexEdgeIds) {
      for (const auto& toVertexEdgeId : toVertexEdgeIds) {
        if (fromVertexEdgeId == toVertexEdgeId) {
          return true;
        }
      }
    }
    return false;
  }

 private:
  int getNextEdgeId() {
    vertexIdCounter_++;
    return vertexIdCounter_ - 1;
  }
  int getNextVertexId() {
    edgeIdCounter_++;
    return edgeIdCounter_ - 1;
  }
  int vertexIdCounter_ = 0;
  int edgeIdCounter_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};
int main() {
  const int number_of_vertices = 18;
  const std::array<std::pair<int, int>, number_of_vertices> connections = {
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
  Graph graph;
  for (int i = 0; i < number_of_vertices; i++) {
    graph.addVertex();
  }
  for (const auto& connection : connections) {
    graph.addEdge(connection.first, connection.second);
  }
  std::ofstream writePT;
  writePT.open("Graph.json", std::ios::out);
  writePT << graph.toString() << std::endl;
  writePT.close();
  return 0;
}
