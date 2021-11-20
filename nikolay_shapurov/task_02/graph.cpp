#include "graph.hpp"

void Graph::add_vertex() {
  static VertexId genId = 0;
  vertexVec_.push_back(Vertex(genId++));
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  static EdgeId genId = 0;
  edgeVec_.push_back(Edge(genId++, from_vertex_id, to_vertex_id));
}
