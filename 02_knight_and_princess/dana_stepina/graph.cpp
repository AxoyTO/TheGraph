#include "graph.hpp"

void Graph::add_vertex() {
  vertices_.emplace_back(get_new_vertex_id());
}
void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert(has_vertex_id(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(has_vertex_id(to_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(!is_connected(from_vertex_id, to_vertex_id) &&
         "These vertices are already connected");

  const auto& new_edge =
      edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id);
  vertices_[from_vertex_id].add_edge_id(new_edge.id);
  vertices_[to_vertex_id].add_edge_id(new_edge.id);
}

bool Graph::has_vertex_id(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_)
    if (vertex_id == vertex.id)
      return true;
  return false;
}
bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(has_vertex_id(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(has_vertex_id(to_vertex_id) &&
         "There is no such vertex in the graph.");

  for (const auto& edge_id_from_vertex :
       vertices_[from_vertex_id].get_edge_ids())
    for (const auto& edge_id_to_vertex : vertices_[to_vertex_id].get_edge_ids())
      if (edge_id_from_vertex == edge_id_to_vertex)
        return true;
  return false;
}
