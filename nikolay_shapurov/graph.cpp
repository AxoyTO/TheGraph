#include "graph.hpp"
#include <assert.h>

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert((from_vertex_id >= 0) &&
         "The from_vertex_id should be greater or equal to 0.");
  assert((to_vertex_id >= 0) &&
         "The to_vertex_id should be greater or equal to 0.");

  assert(has_vertex(from_vertex_id) && "Vertex doesn't exist");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exist");

  assert(!has_edge(from_vertex_id, to_vertex_id) &&
         "Vertices are already connected");

  edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
}

bool Graph::has_vertex(const VertexId& id) const {
  for (const Vertex& vertex : vertices_) {
    if (id == vertex.id) {
      return true;
    }
  }
  return false;
}

bool Graph::has_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) const {
  for (const Edge& edge : edges_) {
    if (from_vertex_id == edge.from_vertex_id &&
        to_vertex_id == edge.to_vertex_id) {
      return true;
    }
  }
  return false;
}
