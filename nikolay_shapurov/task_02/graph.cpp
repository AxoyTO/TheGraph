#include "graph.hpp"

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
}
