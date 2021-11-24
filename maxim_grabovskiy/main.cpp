#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
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
    vector<int> connectedEdges;
    explicit Vertex(VertexId _id) : id(_id) {}
  };
  vector<Vertex> vertexes;
  vector<Edge> edges;
  void addEdge(VertexId fromVertexId, VertexId toVertexId) {
    EdgeId tmp = getNewEdgeId();
    edges.emplace_back(tmp, fromVertexId, toVertexId);
    vertexes[fromVertexId].connectedEdges.push_back(tmp);
    vertexes[toVertexId].connectedEdges.push_back(tmp);
  }
  void addVertex() { vertexes.emplace_back(getNewVertexId()); }
  void spawnVertex(VertexId parentId) {
    addVertex();
    addEdge(parentId, vertexIdCounter_ - 1);
  }

 private:
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
};

class GraphPrinter {
 public:
  GraphPrinter(Graph graph) {
    output << "{\n\"vertices\": [\n";
    for (auto vertIter : graph.vertexes) {
      output << "\t{ \"id\": " << vertIter.id << ", \"edge_ids\":[";
      for (int i = 0; i < (int)vertIter.connectedEdges.size() - 1; i++) {
        output << vertIter.connectedEdges[i] << ", ";
      }
      output << vertIter.connectedEdges[vertIter.connectedEdges.size() - 1]
             << "] }";
      output << ((vertIter.id == (int)graph.vertexes.size() - 1) ? "\n ],\n"
                                                                 : ",\n");
    }
    output << " "
           << "\"edges\": [\n";
    for (auto edgeIter : graph.edges) {
      output << "\t{ \"id\": " << edgeIter.id << ", \"vertex_ids\":["
             << edgeIter.fromVertexId << ", " << edgeIter.toVertexId << "] }";
      output << ((edgeIter.id == (int)graph.edges.size() - 1) ? "\n ]\n}"
                                                              : ",\n");
    }
  };
  string print() const {
    string s = output.str();
    return output.str();
  }

 private:
  std::stringstream output;
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
