#include <cassert>
#include <string>
#include <vector>

#include "graph.hpp"

namespace {

bool is_edge_id_included(
    const uni_cpp_practice::EdgeId& id,
    const std::vector<uni_cpp_practice::EdgeId>& edge_ids) {
  for (const auto& edge_id : edge_ids)
    if (id == edge_id)
      return true;
  return false;
}

using std::min;
using std::to_string;
using std::vector;

}  // namespace

namespace uni_cpp_practice {

void Vertex::add_edge_id(const EdgeId& _id) {
  assert(!is_edge_id_included(_id, edges_ids_));
  edges_ids_.push_back(_id);
}

bool Graph::is_vertex_exist(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_) {
    if (vertex_id == vertex.get_id())
      return true;
  }
  return false;
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(is_vertex_exist(from_vertex_id));
  assert(is_vertex_exist(to_vertex_id));

  const auto& from_vertex_edges_ids = vertices_[from_vertex_id].get_edges_ids();
  const auto& to_vertex_edges_ids = vertices_[to_vertex_id].get_edges_ids();
  for (const auto& from_vertex_edge_id : from_vertex_edges_ids)
    if (from_vertex_id == to_vertex_id) {
      const auto& connected_vertices =
          edges_[from_vertex_edge_id].connected_vertices;
      if (connected_vertices[0] == connected_vertices[1])
        return true;
    } else
      for (const auto& to_vertex_edge_id : to_vertex_edges_ids)
        if (from_vertex_edge_id == to_vertex_edge_id)
          return true;

  return false;
}

void Graph::connect_vertices(const VertexId& from_vertex_id,
                             const VertexId& to_vertex_id,
                             bool initialization) {
  assert(is_vertex_exist(from_vertex_id));
  assert(is_vertex_exist(to_vertex_id));
  assert(!is_connected(from_vertex_id, to_vertex_id));

  if (initialization) {
    const int minimum_depth = [&from_vertex_id, &to_vertex_id,
                               vertices = &vertices_, edges = &edges_]() {
      int min_depth = vertices->at(from_vertex_id).depth;
      for (const auto& edge_idx : vertices->at(to_vertex_id).get_edges_ids()) {
        const VertexId vert = edges->at(edge_idx).connected_vertices[0];
        min_depth = min(min_depth, vertices->at(vert).depth);
      }
      return min_depth;
    }();
    vertices_[to_vertex_id].depth = minimum_depth + 1;
    depth_ = std::max(depth_, minimum_depth + 1);
  }

  const int diff =
      vertices_[to_vertex_id].depth - vertices_[from_vertex_id].depth;

  const Edge::Color color = [&initialization, &diff, &from_vertex_id,
                             &to_vertex_id]() {
    if (initialization)
      return Edge::Color::Gray;
    else if (from_vertex_id == to_vertex_id)
      return Edge::Color::Green;
    else if (diff == 0)
      return Edge::Color::Blue;
    else if (diff == 1)
      return Edge::Color::Yellow;
    else if (diff == 2)
      return Edge::Color::Red;
    else
      return Edge::Color::Gray;
  }();

  const auto& new_edge = edges_.emplace_back(from_vertex_id, to_vertex_id,
                                             get_next_edge_id(), color);
  vertices_[from_vertex_id].add_edge_id(new_edge.id);
  if (from_vertex_id != to_vertex_id)
    vertices_[to_vertex_id].add_edge_id(new_edge.id);
}

std::vector<EdgeId> Graph::get_edge_ids_with_color(
    const Edge::Color& color) const {
  std::vector<EdgeId> edge_ids;
  for (const auto& edge : get_edges()) {
    if (edge.color == color)
      edge_ids.emplace_back(edge.id);
  }

  return edge_ids;
}

}  // namespace uni_cpp_practice
