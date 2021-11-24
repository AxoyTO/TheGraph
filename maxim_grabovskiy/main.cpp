#include <iostream>
#include <vector>
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
  void addEdge(VertexId fromVertexId, VertexId toVertexId) {
    edges.emplace_back(getNewEdgeId(), fromVertexId, toVertexId);
  }
  void addVertex() { vertexes.emplace_back(getNewVertexId()); }
  void spawnVertex(VertexId parentId) {
    addVertex();
    addEdge(parentId, vertexIdCounter_);
  }

 private:
  VertexId vertexIdCounter_ = 0;
  EdgeId edgeIdCounter_ = 0;
  VertexId getNewVertexId() { return vertexIdCounter_++; }
  EdgeId getNewEdgeId() { return edgeIdCounter_++; }
};
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
  return 0;
}
