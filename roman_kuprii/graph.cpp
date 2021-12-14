#include <cassert>
#include <string>
#include <unordered_map>
#include <utility>
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

VertexId Graph::add_vertex() {
  const VertexId new_vertex_id = get_next_vertex_id();
  vertices_map_.emplace(new_vertex_id, new_vertex_id);
  return new_vertex_id;
}

bool Graph::is_vertex_exist(const VertexId& vertex_id) const {
  for (const auto& vertex_pair : vertices_map_) {
    if (vertex_id == vertex_pair.first)
      return true;
  }
  return false;
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(is_vertex_exist(from_vertex_id));
  assert(is_vertex_exist(to_vertex_id));

  const auto& from_vertex_edges_ids =
      vertices_map_.find(from_vertex_id)->second.get_edges_ids();
  const auto& to_vertex_edges_ids =
      vertices_map_.find(to_vertex_id)->second.get_edges_ids();
  for (const auto& from_vertex_edge_id : from_vertex_edges_ids)
    if (from_vertex_id == to_vertex_id) {
      const auto& connected_vertices =
          edges_map_.find(from_vertex_edge_id)->second.connected_vertices;
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
                               vertices_map = &vertices_map_,
                               edges_map = &edges_map_]() {
      int min_depth = vertices_map->find(from_vertex_id)->second.depth;
      for (const auto& edge_id :
           vertices_map->find(to_vertex_id)->second.get_edges_ids()) {
        const VertexId vert =
            edges_map->find(edge_id)->second.connected_vertices[0];
        min_depth = min(min_depth, vertices_map->find(vert)->second.depth);
      }
      return min_depth;
    }();
    vertices_map_.find(to_vertex_id)->second.depth = minimum_depth + 1;

    if (minimum_depth + 1 == depth_map_.size())
      depth_map_.push_back(std::vector<VertexId>({to_vertex_id}));
    else
      depth_map_[minimum_depth + 1].push_back(to_vertex_id);
    depth_ = std::max(depth_, minimum_depth + 1);
  }

  const int diff = vertices_map_.find(to_vertex_id)->second.depth -
                   vertices_map_.find(from_vertex_id)->second.depth;

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

  auto new_edge_id = get_next_edge_id();
  edges_map_.emplace(std::make_pair(
      new_edge_id, Edge(from_vertex_id, to_vertex_id, new_edge_id, color)));
  vertices_map_.find(from_vertex_id)->second.add_edge_id(new_edge_id);
  if (from_vertex_id != to_vertex_id)
    vertices_map_.find(to_vertex_id)->second.add_edge_id(new_edge_id);
}

std::vector<EdgeId> Graph::get_edge_ids_with_color(
    const Edge::Color& color) const {
  std::vector<EdgeId> edge_ids;
  for (const auto& edge_pair : get_edges_map()) {
    if (edge_pair.second.color == color)
      edge_ids.emplace_back(edge_pair.first);
  }

  return edge_ids;
}

const std::vector<VertexId>& Graph::get_vertex_ids_at_depth(int depth) const {
  assert(depth <= depth_map_.size());
  return depth_map_[depth];
}

}  // namespace uni_cpp_practice
