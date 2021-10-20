#include <assert.h>
#include <array>
#include <fstream>
#include <string>
#include <vector>
class Vertex {
 public:
  const int id = 0;
  explicit Vertex(int id) : id(id) {}
  void addEdgeId(int eId) {
    assert(!hasEdgeId(eId) && "Edge Id already exists");
    edgeIds.push_back(eId);
  }
  std::string toString() const {
    std::string str = "";
    str += "\t\t{\"id\":" + std::to_string(id) + ",";
    str += "\"edge_ids\": [";
    for (const auto edgeId : edgeIds) {
      str += std::to_string(edgeId) + ",";
    }
    str.pop_back();
    str += "]}";
    return str;
  }
  bool hasEdgeId(int id) const {
    for (const auto eId : edgeIds) {
      if (id == eId) {
        return true;
      }
    }
    return false;
  }

 private:
  std::vector<int> edgeIds;
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
  void addNode(std::array<int, 3> node) {
    addVertx();
    addEdgeToVer(node.at(0), node.at(1));
    addVertx();
    addEdgeToVer(node.at(2), node.at(1));
    addEdge(node.at(0), node.at(2));
  }
  void addVertx() {
    int vec = getVertex();
    assert(!hasVertex(vec) && "Vertex already exits");
    vertices.emplace_back(vec);
  }
  void addEdgeToVer(int verId, int edgeId) {
    for (auto& vertexId : vertices) {
      if (vertexId.id == verId) {
        vertexId.addEdgeId(edgeId);
      }
    }
  }
  void addEdge(int from, int to) {
    int eId = getEdge();
    edges.emplace_back(eId, from, to);
  }
  std::string toString() {
    // vertex
    std::string strGraph = "";
    strGraph += "{\n\t\"vertices\": [\n";
    for (const auto vertexId : vertices) {
      strGraph += vertexId.toString() + ",\n";
    }
    strGraph.pop_back();
    strGraph.pop_back();
    strGraph += "\n\t],\n";
    // edges
    strGraph += "\t\"edges\": [\n";
    for (const auto edgeId : edges) {
      strGraph += edgeId.toString() + ",\n";
    }
    strGraph.pop_back();
    strGraph.pop_back();
    strGraph += "\n\t]\n}\n";
    return strGraph;
  }
  bool hasVertex(int idFind) {
    for (const auto vertexId : vertices) {
      if (vertexId.id == idFind) {
        return true;
      }
    }
    return false;
  }

 private:
  int getEdge() {
    edgeCount++;
    return edgeCount - 1;
  }
  int getVertex() {
    verCount++;
    return verCount - 1;
  }
  int verCount = 0;
  int edgeCount = 0;
  std::vector<Vertex> vertices;
  std::vector<Edge> edges;
};
int main() {
  const std::array<std::array<int, 3>, 18> netWork = {{{0, 0, 1},
                                                       {0, 1, 2},
                                                       {0, 2, 3},
                                                       {1, 3, 4},
                                                       {1, 4, 5},
                                                       {1, 5, 6},
                                                       {2, 6, 7},
                                                       {2, 7, 8},
                                                       {3, 8, 9},
                                                       {4, 9, 10},
                                                       {5, 10, 10},
                                                       {6, 11, 10},
                                                       {7, 12, 11},
                                                       {8, 13, 11},
                                                       {9, 14, 12},
                                                       {10, 15, 13},
                                                       {11, 16, 13},
                                                       {12, 17, 13}}};
  Graph graph;
  for (const auto netId : netWork) {
    graph.addNode(netId);
  }
  std::ofstream writePT;
  writePT.open("Graphic.json", std::ios::out);
  writePT << graph.toString() << std::endl;
  writePT.close();
  return 0;
}
