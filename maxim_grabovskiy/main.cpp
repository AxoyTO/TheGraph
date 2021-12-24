constexpr float RED_GENERATION_PROBABILITY = 0.33;
constexpr float GREEN_GENERATION_PROBABILITY = 0.1;
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::map;
using std::ofstream;
using std::string;
using std::vector;
using VertexId = int;
using EdgeId = int;
using Depth = int;
bool checkProbability(float probability) {
  std::random_device seed{};
  std::default_random_engine generator(seed());
  std::bernoulli_distribution generationDistribution(probability);
  return generationDistribution(generator);
}
int randomIntNumber(float maximum) {
  std::random_device seed{};
  std::default_random_engine generator(seed());
  std::uniform_int_distribution<> generationDistribution(0, maximum);
  return generationDistribution(generator);
}
class Graph {
 public:
  struct Edge {
    enum class Color { Gray, Green, Yellow, Red };
    EdgeId const id;
    VertexId const fromVertexId;
    VertexId const toVertexId;
    Color const color;
    Edge(EdgeId _id, VertexId _fromVertexId, VertexId _toVertexId, Color _color)
        : id(_id),
          fromVertexId(_fromVertexId),
          toVertexId(_toVertexId),
          color(_color) {}
  };
  struct Vertex {
    VertexId const id;
    explicit Vertex(VertexId _id) : id(_id) {}
  };
  vector<EdgeId> const& getConnections(VertexId vertexId) const {
    return edgeConectionMap_.at(vertexId);
  }
  Depth const& getDepth(VertexId vertexId) const {
    return vertexDepthMap_.at(vertexId);
  }
  void setDepth(VertexId vertexId, Depth depth) {
    vertexDepthMap_[vertexId] = depth;
  }
  vector<EdgeId> const& getVertexIdByDepth(int depth) const {
    return vertexIdByDepth_[depth];
  }
  vector<Vertex> const& getVertexes() const { return vertexes_; }
  vector<Edge> const& getEdges() const { return edges_; }
  Graph::Edge const& getEdge(EdgeId id) const { return edges_[id]; }
  Graph::Vertex const& getVertex(VertexId id) const { return vertexes_[id]; }

  void addEdge(VertexId fromVertexId, VertexId toVertexId) {
    EdgeId const newEdgeId = getNewEdgeId();
    Edge::Color color = calculate_color(fromVertexId, toVertexId);
    const auto& fromVertex = vertexes_[fromVertexId];
    const auto& toVertex = vertexes_[toVertexId];
    edgeConectionMap_[fromVertexId].push_back(newEdgeId);
    if (color != Edge::Color::Green)
      edgeConectionMap_[toVertexId].push_back(newEdgeId);
    if (color == Edge::Color::Gray) {
      for (int i = 0; i < (int)vertexIdByDepth_[0].size() - 1; i++) {
        if (vertexIdByDepth_[0][i] == toVertexId)
          vertexIdByDepth_.erase(vertexIdByDepth_.begin() + i);
      }
      const auto newDepth = getDepth(fromVertex.id) + 1;
      if ((int)vertexIdByDepth_.size() - 1 < newDepth)
        vertexIdByDepth_.emplace_back();

      vertexIdByDepth_[newDepth].emplace_back(toVertexId);
      setDepth(toVertex.id, newDepth);
    }
    edges_.emplace_back(newEdgeId, fromVertexId, toVertexId, color);
  }
  Edge::Color calculate_color(VertexId fromVertexId, VertexId toVertexId) {
    const auto& fromVertex = vertexes_[fromVertexId];
    const auto& toVertex = vertexes_[toVertexId];
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
  VertexId addVertex() {
    VertexId const newVertexId = getNewVertexId();
    auto const& new_vertex = vertexes_.emplace_back(newVertexId);
    if (!newVertexId)
      vertexIdByDepth_.emplace_back();
    vertexIdByDepth_[0].emplace_back(new_vertex.id);
    setDepth(newVertexId, 0);
    edgeConectionMap_[newVertexId] = {};
    return newVertexId;
  }
  VertexId spawnVertex(VertexId parentId) {
    VertexId const newVertexId = addVertex();
    addEdge(parentId, newVertexId);
    return newVertexId;
  }
  bool isConnected(VertexId firstVertexId, VertexId secondVertexId) const {
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

 private:
  vector<Vertex> vertexes_;
  vector<Edge> edges_;
  map<VertexId, Depth> vertexDepthMap_;
  map<VertexId, vector<EdgeId>> edgeConectionMap_;
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
  vector<vector<VertexId>> vertexIdByDepth_;
};
class GraphPrinter {
 public:
  explicit GraphPrinter(Graph const& graph) : graph_(graph) {}
  string print_vertex(Graph::Vertex const& vertex) const {
    std::stringstream vertexOutput;
    vertexOutput << "\t{ \"id\": " << vertex.id << ", \"edge_ids\":[";
    auto const& connections = graph_.getConnections(vertex.id);
    for (int i = 0; i < (int)connections.size(); i++) {
      vertexOutput << connections[i];
      if (i != ((int)(connections.size() - 1)))
        vertexOutput << ", ";
    }
    vertexOutput << "]"
                 << ",\"depth\":" << graph_.getDepth(vertex.id) << "}";
    return vertexOutput.str();
  }

  string print_edge(Graph::Edge const& edge) const {
    std::stringstream edgeOutput;
    edgeOutput << "\t{ \"id\": " << edge.id << ", \"vertex_ids\":["
               << edge.fromVertexId << ", " << edge.toVertexId
               << "],\"color\":\"" << printEdgeColor(edge.color) << "\"}";
    return edgeOutput.str();
  }

  string print() const {
    std::stringstream printOutput;
    printOutput << "{\n\"vertices\": [\n";
    for (auto const& vertex : graph_.getVertexes()) {
      printOutput << print_vertex(vertex);
      printOutput << ",\n";
    }
    if (graph_.getVertexes().size() != 0)
      printOutput.seekp(-2, std::ios_base::end);
    printOutput << "\n\t],\n "
                << "\"edges\": [\n";
    for (auto const& edge : graph_.getEdges()) {
      printOutput << print_edge(edge);
      printOutput << ",\n";
    }
    if (graph_.getEdges().size() != 0)
      printOutput.seekp(-2, std::ios_base::end);
    printOutput << "\n\t]\n}\n";
    return printOutput.str();
  }

 private:
  string printEdgeColor(Graph::Edge::Color const& color) const {
    switch (color) {
      case Graph::Edge::Color::Green:
        return "green";
      case Graph::Edge::Color::Yellow:
        return "yellow";
      case Graph::Edge::Color::Red:
        return "red";
      case Graph::Edge::Color::Gray:
        return "gray";
      default:
        throw std::runtime_error("cannot print color");
    }
  }
  Graph const& graph_;
};

class GraphGenerator {
 public:
  struct Params {
    explicit Params(int _depth = 0, int _new_vertices_num = 0)
        : depth(_depth), new_vertices_num(_new_vertices_num) {}

    const int depth = 0;
    const int new_vertices_num = 0;
  };

  explicit GraphGenerator(const Params& params = Params()) : params_(params) {}

  Graph generate() const {
    auto graph = Graph();
    graph.addVertex();
    generateGrayEdges(graph);
    generateGreenEdges(graph);
    generateYellowEdges(graph);
    generateRedEdges(graph);
    return graph;
  }

 private:
  vector<VertexId> getUnconnectedVertexIds(
      VertexId vertexId,
      std::vector<VertexId> const& vertexIds,
      Graph const& graph) const {
    vector<VertexId> notConnected;
    for (auto randomNextVertexId : vertexIds)
      if (!graph.isConnected(vertexId, randomNextVertexId))
        notConnected.emplace_back(randomNextVertexId);
    return notConnected;
  };
  void generateGrayEdges(Graph& graph) const {
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
  void generateGreenEdges(Graph& graph) const {
    for (Graph::Vertex const& vertex : graph.getVertexes()) {
      if (checkProbability(GREEN_GENERATION_PROBABILITY))
        graph.addEdge(vertex.id, vertex.id);
    }
  }
  void generateYellowEdges(Graph& graph) const {
    float const step = 100.0 / (params_.depth - 1);
    for (auto vertex : graph.getVertexes()) {
      if (checkProbability(
              ((100.0 - (100.0 - (step * (graph.getDepth(vertex.id))))) /
               100)) &&
          graph.getDepth(vertex.id) < params_.depth - 1) {
        auto const nextVertexesIds = getUnconnectedVertexIds(
            vertex.id, graph.getVertexIdByDepth(graph.getDepth(vertex.id) + 1),
            graph);
        if (nextVertexesIds.size() > 0)
          graph.addEdge(
              vertex.id,
              nextVertexesIds[randomIntNumber(nextVertexesIds.size() - 1)]);
      }
    }
  }
  void generateRedEdges(Graph& graph) const {
    for (auto vertex : graph.getVertexes()) {
      if (graph.getDepth(vertex.id) < params_.depth - 2 &&
          checkProbability(RED_GENERATION_PROBABILITY)) {
        auto const nextVertexesIds = getUnconnectedVertexIds(
            vertex.id, graph.getVertexIdByDepth(graph.getDepth(vertex.id) + 2),
            graph);
        if (nextVertexesIds.size() > 0)
          graph.addEdge(
              vertex.id,
              nextVertexesIds[randomIntNumber(nextVertexesIds.size() - 1)]);
      }
    }
  }

 private:
  Params const params_ = Params();
};

int intInput(string inputMessage) {
  int input = -1;
  cout << inputMessage;
  std::cin >> input;
  while ((std::cin.fail() || input < 0)) {
    std::cout << ((input < 0) ? ("Error: input cannot be <0")
                              : "Error: input type != int")
              << "\n";
    std::cin.clear();
    std::cin.ignore(256, '\n');
    cout << "Input " << inputMessage;
    std::cin >> input;
  }
  return input;
};
void writeToFile(string const& output, string const& filename) {
  ofstream fileToWrite(filename);
  fileToWrite << output;
}

int main() {
  int const depth = intInput("depth: ");
  int const newVerticesNum = intInput("new_vertex_num: ");
  auto const params = GraphGenerator::Params(depth, newVerticesNum);
  auto const generator = GraphGenerator(params);
  auto const graph = generator.generate();
  auto const graphPrinter = GraphPrinter(graph);
  auto const graphJson = graphPrinter.print();
  std::cout << graphJson << std::endl;
  writeToFile(graphJson, "graph.json");
  return 0;
}
