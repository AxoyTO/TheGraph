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
  vector<EdgeId> getConnectionsVector(VertexId vertexId) const {
    return edgeConectionMap.at(vertexId);
  }
  vector<Vertex> getVertexesVector() const { return vertexes; }
  vector<Edge> getEdgesVector() const { return edges; }
  void addEdge(VertexId fromVertexId, VertexId toVertexId) {
    EdgeId newEdgeId = getNewEdgeId();
    edgeConectionMap[fromVertexId].push_back(newEdgeId);
    edgeConectionMap[toVertexId].push_back(newEdgeId);
    edges.emplace_back(newEdgeId, fromVertexId, toVertexId);
  }
  VertexId addVertex() {
    VertexId newVertexId = getNewVertexId();
    vertexes.emplace_back(newVertexId);
    edgeConectionMap[newVertexId] = {};
    return newVertexId;
  }
  VertexId spawnVertex(VertexId parentId) {
    VertexId newVertexId = addVertex();
    addEdge(parentId, newVertexId);
    return newVertexId;
  }

 private:
  vector<Vertex> vertexes;
  vector<Edge> edges;
  map<VertexId, vector<EdgeId>> edgeConectionMap;
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
};
class GraphPrinter {
 public:
  explicit GraphPrinter(const Graph& graph) : graph_(graph) {}
  string print_vertex(const Graph::Vertex& vertex) const {
    std::stringstream vertexOutput;
    vertexOutput << "\t{ \"id\": " << vertex.id << ", \"edge_ids\":[";
    const auto& edgeIds = graph_.getConnectionsVector(vertex.id);
    for (int i = 0; i < edgeIds.size(); i++) {
      vertexOutput
          << graph_.getConnectionsVector(vertex.id)[i]
          << ((i != ((int)(graph_.getConnectionsVector(vertex.id).size() - 1)))
                  ? ", "
                  : "]");
    }
    vertexOutput << " }";
    return vertexOutput.str();
  }

  string print_edge(const Graph::Edge& edge) const {
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
    printOutput.seekp(-2, std::ios_base::end);
    printOutput << "\n\t],\n "
                << "\"edges\": [\n";
    for (auto const edge : graph_.getEdgesVector()) {
      printOutput << print_edge(edge);
      printOutput << ",\n";
    }
    printOutput.seekp(-2, std::ios_base::end);
    printOutput << "\n\t]\n}\n";
    string s = printOutput.str();
    return printOutput.str();
  }

 private:
  const Graph& graph_;
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
