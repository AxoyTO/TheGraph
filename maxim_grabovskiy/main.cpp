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
constexpr float RED_GENERATION_PROBABILITY = 0.33;
constexpr float GREEN_GENERATION_PROBABILITY = 0.1;

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
    string getEdgeColor() const {
      switch (color) {
        case Color::Green:
          return "green";
          break;
        case Color::Yellow:
          return "yellow";
          break;
        case Color::Red:
          return "red";
          break;
        default:
          return "gray";
          break;
      }
    }
  };
  struct Vertex {
    VertexId const id;
    int depth;
    explicit Vertex(VertexId _id) : id(_id), depth(0) {}
  };
  vector<EdgeId> const& getconnections(VertexId vertexId) const {
    return edgeConectionMap_.at(vertexId);
  }
  vector<EdgeId> const& getVertexIdByDepth(int depth) const {
    return vertexIdByDepth_[depth];
  }
  vector<Vertex> const& getVertexes() const { return vertexes_; }
  vector<Edge> const& getEdges() const { return edges_; }

  void addEdge(VertexId fromVertexId, VertexId toVertexId, Edge::Color color) {
    EdgeId const newEdgeId = getNewEdgeId();
    edgeConectionMap_[fromVertexId].push_back(newEdgeId);
    edgeConectionMap_[toVertexId].push_back(newEdgeId);
    if (color == Edge::Color::Gray) {
      vertexIdByDepth_[0].pop_back();
      vertexIdByDepth_[vertexes_[fromVertexId].depth + 1].emplace_back(
          toVertexId);
      vertexes_[toVertexId].depth = vertexes_[fromVertexId].depth + 1;
    }
    edges_.emplace_back(newEdgeId, fromVertexId, toVertexId, color);
  }
  VertexId addVertex() {
    VertexId const newVertexId = getNewVertexId();
    auto const& new_vertex = vertexes_.emplace_back(newVertexId);
    vertexIdByDepth_[0].emplace_back(new_vertex.id);
    edgeConectionMap_[newVertexId] = {};
    return newVertexId;
  }
  VertexId spawnVertex(VertexId parentId) {
    VertexId const newVertexId = addVertex();
    addEdge(parentId, newVertexId, Edge::Color::Gray);
    return newVertexId;
  }
  void setVertexIdByDepthSize(int size) { vertexIdByDepth_.resize(size); }
  bool isConnected(VertexId fromVertexId, VertexId toVertexId) const {
    bool isDesceptant = 0;
    auto const& connections = getconnections(fromVertexId);
    for (int i = 0; i < (int)connections.size(); i++)
      isDesceptant = isDesceptant ||
                     (getEdges()[connections[i]].toVertexId == toVertexId) ||
                     (getEdges()[connections[i]].fromVertexId == toVertexId);
    return isDesceptant;
  }
  VertexId getUnconnectedVertexId(VertexId vertexId,
                                  const std::vector<VertexId>& vertexIds) {
    if (vertexIds.size() - 1) {
      VertexId randomNextVertexId;
      do {
        std::random_device seed;
        std::default_random_engine generator(seed());
        std::uniform_int_distribution<> idDistribution(0, vertexIds.size() - 1);
        randomNextVertexId = vertexIds[idDistribution(generator)];
      } while (isConnected(vertexId, randomNextVertexId));
      return randomNextVertexId;
    } else
      return 0;
  }

 private:
  vector<Vertex> vertexes_;
  vector<Edge> edges_;
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
    auto const& connections = graph_.getconnections(vertex.id);
    for (int i = 0; i < (int)connections.size(); i++) {
      vertexOutput << connections[i];
      if (i != ((int)(connections.size() - 1)))
        vertexOutput << ", ";
    }
    vertexOutput << "]"
                 << ",\"depth\":" << vertex.depth << "}";
    return vertexOutput.str();
  }

  string print_edge(Graph::Edge const& edge) const {
    std::stringstream edgeOutput;
    edgeOutput << "\t{ \"id\": " << edge.id << ", \"vertex_ids\":["
               << edge.fromVertexId << ", " << edge.toVertexId
               << "],\"color\":\"" << edge.getEdgeColor() << "\"}";
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
    graph.setVertexIdByDepthSize(params_.depth);
    graph.addVertex();
    generate_gray_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
  }

 private:
  bool probability(float p) const {
    std::random_device seed{};
    std::default_random_engine generator(seed());
    std::bernoulli_distribution generationDistribution(p);
    return generationDistribution(generator);
  }

  void generate_gray_edges(Graph& graph) const {
    float generationProb = 0;
    bool generated = 0;
    float const step = 100.0 / (params_.depth - 1);
    for (int currentDepth = 0; currentDepth < params_.depth; currentDepth++) {
      generated = 0;
      generationProb = ((100.0 - (step * (currentDepth))) / 100);
      for (auto parrentId : graph.getVertexIdByDepth(currentDepth)) {
        for (int j = 0; j < params_.new_vertices_num; j++) {
          if (probability(generationProb)) {
            graph.spawnVertex(parrentId);
            generated = 1;
          }
          if (!generated)
            break;
        }
      }
    }
  }
  void generate_green_edges(Graph& graph) const {
    for (auto vertex : graph.getVertexes()) {
      if (probability(GREEN_GENERATION_PROBABILITY))
        graph.addEdge(vertex.id, vertex.id, Graph::Edge::Color::Green);
    }
  }
  void generate_yellow_edges(Graph& graph) const {
    float const step = 100.0 / (params_.depth - 1);
    for (auto vertex : graph.getVertexes()) {
      if (probability(((100.0 - (100.0 - (step * (vertex.depth)))) / 100)) &&
          vertex.depth < params_.depth - 1) {
        auto randomNextVertexId = graph.getUnconnectedVertexId(
            vertex.id, graph.getVertexIdByDepth(vertex.depth + 1));
        if (randomNextVertexId)
          graph.addEdge(vertex.id, randomNextVertexId,
                        Graph::Edge::Color::Yellow);
      }
    }
  }
  void generate_red_edges(Graph& graph) const {
    for (auto vertex : graph.getVertexes()) {
      if (vertex.depth < params_.depth - 2 &&
          probability(RED_GENERATION_PROBABILITY)) {
        auto randomNextVertexId = graph.getUnconnectedVertexId(
            vertex.id, graph.getVertexIdByDepth(vertex.depth + 2));
        if (randomNextVertexId)
          graph.addEdge(vertex.id, randomNextVertexId, Graph::Edge::Color::Red);
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
    std::cout << ((input < 0) ? "Error: <0" : "Error: !=int") << "\n";
    std::cin.clear();
    std::cin.ignore(256, '\n');
    cout << inputMessage;
    std::cin >> input;
  }
  return input;
};
void write_to_file(string const& output, string const& filename) {
  ofstream fileToWrite(filename);
  fileToWrite << output;
}
int main() {
  int const depth = intInput("Input depth: ");
  int const new_vertices_num = intInput("Input new_vertex_num: ");
  auto const params = GraphGenerator::Params(depth, new_vertices_num);
  auto const generator = GraphGenerator(params);
  auto const graph = generator.generate();
  auto const graph_printer = GraphPrinter(graph);
  auto const graph_json = graph_printer.print();
  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");
  return 0;
}
