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

class Graph {
 public:
  struct Edge {
    EdgeId const id;
    VertexId const fromVertexId;
    VertexId const toVertexId;
    string const color;
    Edge(EdgeId _id,
         VertexId _fromVertexId,
         VertexId _toVertexId,
         string _color)
        : id(_id),
          fromVertexId(_fromVertexId),
          toVertexId(_toVertexId),
          color(_color) {}
  };
  struct Vertex {
    VertexId const id;
    int depth;
    explicit Vertex(VertexId _id, int _depth) : id(_id), depth(_depth) {}
  };
  vector<EdgeId> const& getConnectionsVector(VertexId vertexId) const {
    return edgeConectionMap_.at(vertexId);
  }
  vector<EdgeId> const& getVertexIdByDepth(int depth) const {
    return vertexIdByDepth_.at(depth);
  }
  vector<Vertex> const& getVertexes() const { return vertexes_; }
  vector<Edge> const& getEdges() const { return edges_; }

  void addEdge(VertexId fromVertexId, VertexId toVertexId, string color) {
    EdgeId const newEdgeId = getNewEdgeId();
    edgeConectionMap_[fromVertexId].push_back(newEdgeId);
    edgeConectionMap_[toVertexId].push_back(newEdgeId);
    edges_.emplace_back(newEdgeId, fromVertexId, toVertexId, color);
  }
  VertexId addVertex() {
    VertexId const newVertexId = getNewVertexId();
    vertexes_.emplace_back(newVertexId, 0);
    vertexIdByDepth_[0].emplace_back(0);
    edgeConectionMap_[newVertexId] = {};
    return newVertexId;
  }
  VertexId spawnVertex(VertexId parentId) {
    VertexId const newVertexId = addVertex();
    vertexIdByDepth_[0].pop_back();
    vertexIdByDepth_[vertexes_[parentId].depth + 1].emplace_back(newVertexId);
    vertexes_[newVertexId].depth = vertexes_[parentId].depth + 1;
    addEdge(parentId, newVertexId, "gray");
    return newVertexId;
  }

 private:
  vector<Vertex> vertexes_;
  vector<Edge> edges_;
  map<VertexId, vector<EdgeId>> edgeConectionMap_;
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
  map<int, vector<VertexId>> vertexIdByDepth_;
};
class GraphPrinter {
 public:
  explicit GraphPrinter(Graph const& graph) : graph_(graph) {}
  string print_vertex(Graph::Vertex const& vertex) const {
    std::stringstream vertexOutput;
    vertexOutput << "\t{ \"id\": " << vertex.id << ", \"edge_ids\":[";
    auto const& connectionsVector = graph_.getConnectionsVector(vertex.id);
    for (int i = 0; i < (int)connectionsVector.size(); i++) {
      vertexOutput << connectionsVector[i];
      if (i != ((int)(connectionsVector.size() - 1)))
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
               << "],\"color\":\"" << edge.color << "\"}";
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
    graph.addVertex();
    generate_gray_edges(graph);
    generate_green_edges(graph);
    generate_yellow_edges(graph);
    generate_red_edges(graph);
    return graph;
  }
  void generate_gray_edges(Graph& graph) const {
    float generationProb = 0;
    float const step = 100.0 / (params_.depth - 1);
    std::random_device seed{};
    std::default_random_engine generator(seed());
    std::bernoulli_distribution grayGenerationDistribution;
    for (int currentDepth = 0; currentDepth < params_.depth; currentDepth++) {
      generationProb = ((100.0 - (step * (currentDepth))) / 100);
      grayGenerationDistribution = std::bernoulli_distribution(generationProb);
      for (auto parrentId : graph.getVertexIdByDepth(currentDepth)) {
        for (int j = 0; j < params_.new_vertices_num; j++) {
          if (grayGenerationDistribution(generator))
            graph.spawnVertex(parrentId);
        }
      }
    }
  }
  void generate_green_edges(Graph& graph) const {
    std::random_device seed{};
    std::default_random_engine generator(seed());
    std::bernoulli_distribution greenGenerationDistribution(0.1);
    for (auto vertex : graph.getVertexes()) {
      if (greenGenerationDistribution(generator))
        graph.addEdge(vertex.id, vertex.id, "green");
    }
  }
  void generate_yellow_edges(Graph& graph) const {
    bool isDesceptant = 0;
    float const step = 100.0 / (params_.depth - 1);
    std::random_device seed{};
    std::default_random_engine generator(seed());
    std::bernoulli_distribution yellowGenerationDistribution;
    for (auto vertex : graph.getVertexes()) {
      yellowGenerationDistribution = std::bernoulli_distribution(
          (100.0 - (100.0 - (step * (vertex.depth)))) / 100);
      if (yellowGenerationDistribution(generator) &&
          vertex.depth < params_.depth - 1) {
        isDesceptant = 0;
        auto randomPreviousVertexId = graph.getVertexIdByDepth(
            vertex.depth +
            1)[rand() % graph.getVertexIdByDepth(vertex.depth + 1).size()];
        auto const& connectionsVector = graph.getConnectionsVector(vertex.id);
        for (int i = 0; i < (int)connectionsVector.size(); i++)
          isDesceptant = isDesceptant ||
                         (graph.getEdges()[connectionsVector[i]].toVertexId ==
                          randomPreviousVertexId) ||
                         (graph.getEdges()[connectionsVector[i]].fromVertexId ==
                          randomPreviousVertexId);

        if (!isDesceptant)
          graph.addEdge(vertex.id, randomPreviousVertexId, "yellow");
      }
    }
  }
  void generate_red_edges(Graph& graph) const {
    std::random_device seed{};
    std::default_random_engine generator(seed());
    std::bernoulli_distribution redGenerationDistribution(0.33);
    for (auto vertex : graph.getVertexes()) {
      if (redGenerationDistribution(generator) &&
          vertex.depth < params_.depth - 2) {
        auto randomPreviousVertexId = graph.getVertexIdByDepth(
            vertex.depth +
            2)[rand() % graph.getVertexIdByDepth(vertex.depth + 1).size()];
        graph.addEdge(vertex.id, randomPreviousVertexId, "red");
      }
    }
  }

 private:
  const Params params_ = Params();
};

int const intInput() {
  int input = -1;
  std::cin >> input;
  while ((std::cin.fail() || input < 0)) {
    std::cout << "error"
              << "\n";
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cin >> input;
  }
  return input;
};
void write_to_file(string const& output, string const& filename) {
  ofstream fileToWrite(filename);
  fileToWrite << output;
}
int main() {
  const int depth = intInput();
  const int new_vertices_num = intInput();
  const auto params = GraphGenerator::Params(depth, new_vertices_num);
  const auto generator = GraphGenerator(params);
  const auto graph = generator.generate();
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();

  std::cout << graph_json << std::endl;
  write_to_file(graph_json, "graph.json");

  return 0;
}
