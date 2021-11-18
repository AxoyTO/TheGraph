/**
 * Not done yet no need to check !!!
 * done now:
 *  change class Edge
 *  change class Vertex
 *  change class Graph
 * todo:
 * generator ..............todo
 * main func...............todo
 * comment: form 19/11/2021 >too late now to dont it.........
 */
#include <assert.h>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
constexpr float GREEN_EDGE_PROBABILITY = 0.1;
constexpr float BLUE_EDGE_PROBABILITY = 0.25;
constexpr float RED_EDGE_PROBABILITY = 0.33;

class Vertex {
 public:
  const int id = 0;
  const int depth = 0;
  explicit Vertex(int id, int depth) : id(id), depth(depth) {}
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
    str += "], \"depth\": ";
    str += std::to_string(depth) + "}";
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
  const std::string color = "NULL";
  Edge(int id, int fromVertexId, int toVertexId, std::string color)
      : id(id),
        fromVertexId(fromVertexId),
        toVertexId(toVertexId),
        color(color) {}
  std::string toString() const {
    std::string strEdge = "";
    strEdge += "\t\t{\"id\": " + std::to_string(id) + ",";
    strEdge += "\"vertex_ids\": [" + std::to_string(fromVertexId) + "," +
               std::to_string(toVertexId) + "], color:\"" + color + "\" }";
    return strEdge;
  }
};
class Graph {
 public:
  void addVertex(int depth) {
    vertices_.emplace_back(getNextVertexId(), depth);
  }
  int getPresentVertexId() { return vertexIdCounter_; }
  void addEdge(int fromVertexId, int toVertexId, std::string color) {
    assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
    assert(hasVertex(toVertexId) && "Vertex doesn't exist");
    assert(!isConnected(fromVertexId, toVertexId) &&
           "Vertices already connected");
    const auto& newEdge =
        edges_.emplace_back(getNextEdgeId(), fromVertexId, toVertexId, color);
    getVertex(fromVertexId).addEdgeId(newEdge.id);
    getVertex(toVertexId).addEdgeId(newEdge.id);
  }
  std::string toString() const {
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
    assert(hasVertex(fromVertexId) && "Vertex doesn't exist");
    assert(hasVertex(toVertexId) && "Vertex doesn't exist");
    for (const auto& fromVertexEdgeId : getVertex(fromVertexId).getEdgeIds()) {
      for (const auto& toVertexEdgeId : getVertex(toVertexId).getEdgeIds()) {
        if (fromVertexEdgeId == toVertexEdgeId) {
          return true;
        }
      }
    }
    return false;
  }

 private:
  Vertex& getVertex(int id) {
    assert(hasVertex(id) && "Vertex doesn't exist");
    for (auto& vertex : vertices_) {
      if (vertex.id == id) {
        return vertex;
      }
    }
    throw std::runtime_error("Unreachable code");
  }
  int getNextEdgeId() { return vertexIdCounter_++; }
  int getNextVertexId() { return edgeIdCounter_++; }
  int vertexIdCounter_ = 0;
  int edgeIdCounter_ = 0;
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
};
class Generator {
 public:
  Generator(int maxDepth, int newVerticesNum)
      : maxDeps_(maxDepth), newVerticesNum_(newVerticesNum) {}
  // Recursion to create a table for default edge to connect them done
  void verticesGenerator(int presentDepth = 0) {
    for (int i = 0; i < newVerticesNum_; ++i) {
      if (presentDepth < maxDeps_ &&
          vertxGenerateProbabilityController(presentDepth)) {
        int formVertex = graph.getPresentVertexId();
        graph.addVertex(presentDepth);
        int toVertexId = graph.getPresentVertexId();
        std::vector<int> connection{formVertex, toVertexId, presentDepth};
        connections_.push_back(connection);
        verticesGenerator(presentDepth++);
      }
    }
  }
  // done with this function still need rgb to input
  void grayEdgeGenerator() {
    std::string color = "gray";
    for (const auto& connection : connections_) {
      graph.addEdge(connection.at(0), connection.at(1), color);
    }
  }
  // done with this func
  bool vertxGenerateProbabilityController(int depth) {
    float probability = (maxDeps_ - depth) / maxDeps_ * 100;
    srand((unsigned)time(NULL));          // default time clock for random
    int randomNumber = rand() % 100 + 1;  // random from 1 to 100
    if (randomNumber > probability) {
      return false;
    }
    return true;
  }

 private:
  Graph graph;
  std::vector<std::vector<int>> connections_;
  int maxDeps_;
  int newVerticesNum_;
};
int main() {
  int maxDeep = 0, newVerticesNum = 0;
  std::cout << "Enter Max Depth:";
  std::cin >> maxDeep;
  std::cout << "Enter new vertices num:";
  std::cin >> newVerticesNum;
  if (newVerticesNum <= 0 || maxDeep <= 0) {
    throw std::runtime_error("Wrong number");
  }
  const int numberOfVertices = 18;
  const int verticesCount = 14;
  const std::array<std::pair<int, int>, numberOfVertices> connections = {
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
  for (int i = 0; i < verticesCount; i++) {
    // graph.addVertex();
  }
  for (const auto& connection : connections) {
    // graph.addEdge(connection.first, connection.second);
  }
  std::ofstream writePT;
  writePT.open("Graph.json", std::ios::out);
  writePT << graph.toString() << std::endl;
  writePT.close();
  return 0;
}
