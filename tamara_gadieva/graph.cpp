#include "graph.hpp"
#include <cassert>
#include <stdexcept>

namespace uni_cource_cpp {

const Edge& Graph::get_edge(const EdgeId& id) const {
  for (const auto& edge : edges_)
    if (edge.id == id)
      return edge;
  throw std::runtime_error("Unreachable code");
}

const Vertex& Graph::get_vertex(const VertexId& id) const {
  assert(has_vertex(id) && "Vertex doesn't exists");
  for (const auto& vertex : vertices_)
    if (vertex.id == id)
      return vertex;
  throw std::runtime_error("Unreachable code");
}

Vertex& Graph::get_vertex(const VertexId& id) {
  const auto& const_self = *this;
  return const_cast<Vertex&>(const_self.get_vertex(id));
}

bool Vertex::has_edge_id(const EdgeId& id) const {
  for (const auto& edge_id : edge_ids_)
    if (edge_id == id)
      return true;
  return false;
}

void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id) && "Vertex already has this edge id");
  edge_ids_.push_back(id);
}

bool Graph::has_vertex(const VertexId& id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == id)
      return true;
  return false;
}

const std::vector<VertexId>& Graph::get_vertex_ids_in_depth(int depth) const {
  assert((depth < get_depth()) && "Invalid depth");
  return vertices_ids_in_depth_[depth];
}

void Graph::update_vertex_depth(const VertexId& id, int depth) {
  assert(depth <= get_depth() && "Invalid depth");
  if (depth == get_depth())
    vertices_ids_in_depth_.emplace_back();
  vertices_ids_in_depth_[depth].push_back(id);
  auto& vertex = get_vertex(id);
  vertex.depth = depth;
  auto& ids = vertices_ids_in_depth_[0];
  for (auto vertex_id_ind = ids.begin() + 1; vertex_id_ind < ids.end();
       vertex_id_ind++) {
    if (*vertex_id_ind == id)
      ids.erase(vertex_id_ind);
  }
}

VertexId Graph::add_vertex() {
  const auto new_vertex_id = get_new_vertex_id();
  vertices_.emplace_back(new_vertex_id);
  if (vertices_ids_in_depth_.size() == 0)
    vertices_ids_in_depth_.emplace_back();
  vertices_ids_in_depth_[0].push_back(new_vertex_id);
  return new_vertex_id;
}

const std::vector<EdgeId>& Graph::get_edge_ids(const VertexId& id) const {
  const auto& vertex = get_vertex(id);
  return vertex.get_edge_ids();
}

Edge::Color Graph::define_edge_color(const VertexId& from_vertex_id,
                                     const VertexId& to_vertex_id) {
  auto& from_vertex = get_vertex(from_vertex_id);
  auto& to_vertex = get_vertex(to_vertex_id);
  if (get_edge_ids(to_vertex_id).size() == 0)
    return Edge::Color::Gray;
  if (from_vertex_id == to_vertex_id)
    return Edge::Color::Green;
  if (to_vertex.depth - from_vertex.depth == 1 &&
      !is_connected(from_vertex_id, to_vertex_id))
    return Edge::Color::Yellow;
  if (to_vertex.depth - from_vertex.depth == 2)
    return Edge::Color::Red;
  throw std::runtime_error("Unreachable code");
}

const std::vector<EdgeId>& Graph::get_colored_edges(
    const Edge::Color& color) const {
  if (edges_color_map_.find(color) == edges_color_map_.end()) {
    static std::vector<EdgeId> empty_result;
    return empty_result;
  }
  return edges_color_map_.at(color);
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert(has_vertex(from_vertex_id) && "Vertex doesn't exist");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exist");
  assert(!is_connected(from_vertex_id, to_vertex_id) &&
         "Vertices are already connected");
  auto& from_vertex = get_vertex(from_vertex_id);
  auto& to_vertex = get_vertex(to_vertex_id);
  const auto edge_color = define_edge_color(from_vertex_id, to_vertex_id);
  const auto& new_edge = edges_.emplace_back(get_new_edge_id(), from_vertex_id,
                                             to_vertex_id, edge_color);
  edges_color_map_[edge_color].push_back(new_edge.id);
  from_vertex.add_edge_id(new_edge.id);
  if (edge_color != Edge::Color::Green)
    to_vertex.add_edge_id(new_edge.id);
  if (edge_color == Edge::Color::Gray) {
    const int new_depth = from_vertex.depth + 1;
    update_vertex_depth(to_vertex.id, new_depth);
  }
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exist");
  const auto& from_vertex = get_vertex(from_vertex_id);
  const auto& to_vertex = get_vertex(to_vertex_id);
  const auto& from_vertex_edges_ids = from_vertex.get_edge_ids();
  const auto& to_vertex_edges_ids = to_vertex.get_edge_ids();
  if (from_vertex_id != to_vertex_id) {
    for (const auto& from_vertex_edge_id : from_vertex_edges_ids)
      for (const auto& to_vertex_edge_id : to_vertex_edges_ids)
        if (from_vertex_edge_id == to_vertex_edge_id)
          return true;
  } else {
    for (const auto& from_vertex_edge_id : from_vertex_edges_ids) {
      const auto& edge = get_edge(from_vertex_edge_id);
      if (edge.color == Edge::Color::Green)
        return true;
    }
  }
  return false;
}
}  // namespace uni_cource_cpp
