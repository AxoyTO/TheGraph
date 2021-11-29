#include "graph.hpp"
#include <algorithm>
#include <cassert>

void Graph::add_vertex() {
  const VertexId new_vertex_id = get_new_vertex_id();
  vertices_.emplace_back(new_vertex_id);
  adjacency_map_[new_vertex_id] = {};
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

  const int new_edge_id = get_new_edge_id();
  edges_.emplace_back(new_edge_id, from_vertex_id, to_vertex_id);

  adjacency_map_[from_vertex_id].emplace_back(new_edge_id);
  adjacency_map_[to_vertex_id].emplace_back(new_edge_id);
}

const std::vector<EdgeId>& Graph::get_edge_ids(const VertexId& id) const {
  return adjacency_map_.at(id);
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
  for (const auto& from_edge_id : adjacency_map_.at(from_vertex_id)) {
    for (const auto& to_edge_id : adjacency_map_.at(to_vertex_id)) {
      if (from_edge_id == to_edge_id) {
        return true;
      }
    }
  }
  return false;
}
