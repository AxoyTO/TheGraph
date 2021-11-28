#include <fstream>
#include <iostream>
#include <map>
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
    const EdgeId id;
    const VertexId fromVertexId;
    const VertexId toVertexId;
    Edge(EdgeId _id, VertexId _fromVertexId, VertexId _toVertexId)
        : id(_id), fromVertexId(_fromVertexId), toVertexId(_toVertexId) {}
  };
  struct Vertex {
    const VertexId id;
    explicit Vertex(VertexId _id) : id(_id) {}
  };
  vector<EdgeId> const& getConnectionsVector(VertexId vertexId) const {
    return edgeConectionMap_.at(vertexId);
  }
  vector<Vertex> getVertexesVector() const { return vertexes_; }
  vector<Edge> getEdgesVector() const { return edges_; }
  void addEdge(VertexId fromVertexId, VertexId toVertexId) {
    const EdgeId newEdgeId = getNewEdgeId();
    edgeConectionMap_[fromVertexId].push_back(newEdgeId);
    edgeConectionMap_[toVertexId].push_back(newEdgeId);
    edges_.emplace_back(newEdgeId, fromVertexId, toVertexId);
  }
  VertexId addVertex() {
    const VertexId newVertexId = getNewVertexId();
    vertexes_.emplace_back(newVertexId);
    edgeConectionMap_[newVertexId] = {};
    return newVertexId;
  }
  VertexId spawnVertex(VertexId parentId) {
    const VertexId newVertexId = addVertex();
    addEdge(parentId, newVertexId);
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
};
class GraphPrinter {
 public:
  explicit GraphPrinter(Graph const& graph) : graph_(graph) {}
  string print_vertex(Graph::Vertex const& vertex) const {
    std::stringstream vertexOutput;
    vertexOutput << "\t{ \"id\": " << vertex.id << ", \"edge_ids\":[";
    const auto& connectionsVector = graph_.getConnectionsVector(vertex.id);
    for (int i = 0; i < (int)connectionsVector.size(); i++) {
      vertexOutput << connectionsVector[i];
      if (i != ((int)(connectionsVector.size() - 1)))
        vertexOutput << ", ";
    }
    vertexOutput << "]"
                 << " }";
    return vertexOutput.str();
  }

  string print_edge(Graph::Edge const& edge) const {
    std::stringstream edgeOutput;
    edgeOutput << "\t{ \"id\": " << edge.id << ", \"vertex_ids\":["
               << edge.fromVertexId << ", " << edge.toVertexId << "] }";
    return edgeOutput.str();
  }

  string print() const {
    std::stringstream printOutput;
    printOutput << "{\n\"vertices\": [\n";
    for (auto const vertex : graph_.getVertexesVector()) {
      printOutput << print_vertex(vertex);
      printOutput << ",\n";
    }
    if (graph_.getVertexesVector().size() != 0)
      printOutput.seekp(-2, std::ios_base::end);
    printOutput << "\n\t],\n "
                << "\"edges\": [\n";
    for (auto const edge : graph_.getEdgesVector()) {
      printOutput << print_edge(edge);
      printOutput << ",\n";
    }
    if (graph_.getEdgesVector().size() != 0)
      printOutput.seekp(-2, std::ios_base::end);
    printOutput << "\n\t]\n}\n";
    string s = printOutput.str();
    return printOutput.str();
  }

 private:
  Graph const& graph_;
};

void write_to_file(string const& output, string const& filename) {
  ofstream fileToWrite(filename);
  fileToWrite << output;
}

int main() {
  Graph graph;
  graph.addVertex();
  graph.spawnVertex(0);
  graph.spawnVertex(0);
  graph.spawnVertex(0);
  graph.spawnVertex(1);
  graph.spawnVertex(1);
  graph.spawnVertex(1);
  graph.spawnVertex(2);
  graph.spawnVertex(2);
  graph.spawnVertex(3);
  graph.spawnVertex(5);
  graph.spawnVertex(7);
  graph.spawnVertex(9);
  graph.spawnVertex(10);
  graph.addEdge(4, 10);
  graph.addEdge(6, 10);
  graph.addEdge(8, 11);
  graph.addEdge(11, 13);
  graph.addEdge(12, 13);
  const auto graph_printer = GraphPrinter(graph);
  const auto graph_json = graph_printer.print();
  cout << graph_json;
  write_to_file(graph_json, "graph.json");
  return 0;
}
