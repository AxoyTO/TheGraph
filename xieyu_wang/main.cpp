/**
 * Not done yet no need to check !!!
 * done now:
 *  change class Edge
 *  change class Vertex
 *  change class Graph
 *  vertexGenerator done
 *  generator gray edge
 *  green done
 * todo:
 * redGenerator ..............todo
 * blueGenerator..............toFix
 * yellowGenerator............todo
 */
#include <assert.h>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
constexpr float GREEN_EDGE_PROBABILITY = 10;
constexpr float BLUE_EDGE_PROBABILITY = 25;
constexpr float RED_EDGE_PROBABILITY = 33;

class Vertex {
 public:
  const int id = 0;
  const int depth = 0;
  explicit Vertex(int id, int depth) : id(id), depth(depth) {}
  void addEdgeId(int eId) { edgeIds_.push_back(eId); }
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
               std::to_string(toVertexId) + "], \"color\":\"" + color + "\" }";
    return strEdge;
  }
};
class Graph {
 public:
  void addVertex(int depth) {
    vertices_.emplace_back(getNextVertexId(), depth);
  }
  int getPresentVertexId() { return edgeIdCounter_; }
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
      : maxDeps_(maxDepth), newVerticesNum_(newVerticesNum) {
    srand((unsigned)time(NULL));
  }
  void verticesGenerator(int presentDepth = 1, int fromVertexId = 0) {
    std::vector<int> ElementPresentLevel;
    if (graph_.getPresentVertexId() == 0) {
      graph_.addVertex(0);
    }
    if (presentDepth < maxDeps_) {
      for (int i = 0; i < newVerticesNum_; ++i) {
        if (vertxGenerateProbabilityController(presentDepth)) {
          graph_.addVertex(presentDepth);
          ElementPresentLevel.push_back(graph_.getPresentVertexId() - 1);
          std::vector<int> connection{
              fromVertexId, graph_.getPresentVertexId() - 1, presentDepth};
          connections_.push_back(connection);
        }
      }
      for (const auto& vertex : ElementPresentLevel) {
        verticesGenerator(presentDepth + 1, vertex);
      }
    }
  }
  void grayEdgeGenerator() {
    std::string color = "gray";
    for (const auto& connection : connections_) {
      graph_.addEdge(connection.at(0), connection.at(1), color);
    }
  }
  /**
   * to setup levels for easy create rgb edge
   */
  void setUpLevels() {
    for (int i = 0; i < maxDeps_; ++i) {
      std::vector<int> level{};
      for (const auto& connection : connections_) {
        if (connection.at(2) == i) {
          level.push_back(connection.at(1));
        }
      }
      levels_.push_back(level);
    }
  }
  /**
   * connect level +2
   * param: NULL
   * return NULL
   */
  void redEdgeGenerator() {}
  /**
   * create edge vertex self
   * param: NULL
   * return NULL
   */
  void greenEdgeGenerator() {
    int verticesSum = graph_.getPresentVertexId();
    for (int i = 0; i < verticesSum; ++i) {
      if (specialEdgeGenerateProbabilityController("green")) {
        graph_.addEdge(i, i, "green");
      }
    }
  }
  /**
   * create edge at same level
   * param: null
   * return:null
   * bug need fix
   **/
  void blueEdgeGenerator() {
    for (const auto& level : levels_) {
      for (auto iter = level.begin(); iter < level.end() - 1; iter++) {
        if (iter != level.end() &&
            specialEdgeGenerateProbabilityController("blue")) {
          graph_.addEdge(*iter, *(iter + 1), "blue");
        }
      }
    }
  }
  bool specialEdgeGenerateProbabilityController(std::string color) {
    int randomNum = rand() % 100 + 1;
    if (color == "red") {
      if (randomNum > RED_EDGE_PROBABILITY) {
        return false;
      }
    } else if (color == "blue") {
      if (randomNum > BLUE_EDGE_PROBABILITY) {
        return false;
      }
    } else if (color == "green") {
      if (randomNum > GREEN_EDGE_PROBABILITY) {
        return false;
      }
    } else {
      std::cerr << "Unknown color!!";
      throw std::error_code();
    }
    return true;
  }
  bool vertxGenerateProbabilityController(int depth) {
    float probability = (maxDeps_ - depth);
    probability = probability / maxDeps_ * 100;
    int randomNumber = rand() % 100 + 1;  // random from 1 to 100
    if (randomNumber > probability) {
      return false;
    }
    return true;
  }
  std::string getResult() { return graph_.toString(); }

 private:
  std::vector<std::vector<int>> levels_;  // for better generate grb
  Graph graph_;
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
  Generator generator(maxDeep, newVerticesNum);
  std::cout << "Generator.................done" << std::endl;
  generator.verticesGenerator();
  std::cout << "VerticesGenerate..........done" << std::endl;
  generator.setUpLevels();
  std::cout << "setUpLevels...............done" << std::endl;
  generator.grayEdgeGenerator();
  std::cout << "GrayEdgeGenerate..........done" << std::endl;
  generator.greenEdgeGenerator();
  std::cout << "GreenEdgeGenerate.........done" << std::endl;
  generator.blueEdgeGenerator();
  std::cout << "BlueEdgeGenerate.........done" << std::endl;
  std::ofstream writePT;
  writePT.open("Graph.json", std::ios::out);
  writePT << generator.getResult() << std::endl;
  writePT.close();
  return 0;
}
