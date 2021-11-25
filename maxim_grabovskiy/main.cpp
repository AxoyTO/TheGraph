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
  vector<Vertex> vertexes;
  vector<Edge> edges;
  map<VertexId, vector<EdgeId>> edgeConectionMap;
  void addEdge(VertexId fromVertexId, VertexId toVertexId) {
    EdgeId tempEdgeId = getNewEdgeId();
    edgeConectionMap[fromVertexId].push_back(tempEdgeId);
    edgeConectionMap[toVertexId].push_back(tempEdgeId);
    edges.emplace_back(tempEdgeId, fromVertexId, toVertexId);
  }
  VertexId addVertex() {
    vector<EdgeId> tempEdgeIdVector;
    VertexId tempVertexId = getNewVertexId();
    vertexes.emplace_back(tempVertexId);
    edgeConectionMap.insert(make_pair(tempVertexId, tempEdgeIdVector));
    return tempVertexId;
  }
  VertexId spawnVertex(VertexId parentId) {
    VertexId tempVertexId = addVertex();
    addEdge(parentId, tempVertexId);
    return tempVertexId;
  }

 private:
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
};
class GraphPrinter {
 public:
  GraphPrinter(const Graph& graph) : graph_(graph) {}
  string print_vertex(const Graph::Vertex& vertex) const {
    std::stringstream vertexOutput;
    vertexOutput << "\t{ \"id\": " << vertex.id << ", \"edge_ids\":[";
    for (int i = 0;
         i < ((int)(graph_.edgeConectionMap.at(vertex.id).size() - 1)); i++) {
      vertexOutput << graph_.edgeConectionMap.at(vertex.id)[i] << ", ";
    }
    vertexOutput
        << graph_.edgeConectionMap.at(
               vertex.id)[graph_.edgeConectionMap.at(vertex.id).size() - 1]
        << "] }";
    vertexOutput << ((vertex.id == (int)graph_.vertexes.size() - 1) ? "\n"
                                                                    : ",\n");
    return vertexOutput.str();
  }

  string print_edge(const Graph::Edge& edge) const {
    std::stringstream edgeOutput;
    edgeOutput << "\t{ \"id\": " << edge.id << ", \"vertex_ids\":["
               << edge.fromVertexId << ", " << edge.toVertexId << "] }";
    edgeOutput << ((edge.id == (int)graph_.edges.size() - 1) ? "\n ]\n}\n"
                                                             : ",\n");
    return edgeOutput.str();
  }

  string print() const {
    std::stringstream printOutput;
    printOutput << "{\n\"vertices\": [\n";
    for (auto const vertex : graph_.vertexes) {
      printOutput << print_vertex(vertex);
    }
    printOutput << "\t],\n "
                << "\"edges\": [\n";
    for (auto const edge : graph_.edges) {
      printOutput << print_edge(edge);
    }
    string s = printOutput.str();
    return printOutput.str();
  }

 private:
  const Graph& graph_;
};
void write_to_file(string output, string filename) {
  freopen("graph.json", "w", stdout);

  cout << output;
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
