#include <cassert>
#include <string>
#include <vector>

#include "graph.hpp"

namespace uni_cpp_practice {

using std::min;
using std::to_string;
using std::vector;

using EdgeId = int;
using VertexId = int;

std::string color_to_string(const Color& color) {
  switch (color) {
    case Color::Gray:
      return "\"gray\" }";
    case Color::Green:
      return "\"green\" }";
    case Color::Blue:
      return "\"blue\" }";
    case Color::Yellow:
      return "\"yellow\" }";
    case Color::Red:
      return "\"red\" }";
  }
}

std::string Edge::to_json() const {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(id);
  res += ", \"vertex_ids\": [";
  res += to_string(connected_vertices[0]);
  res += ", ";
  res += to_string(connected_vertices[1]);
  res += "], \"color\": ";
  res += color_to_string(color);
  return res;
}

bool is_edge_id_included(const EdgeId& id, const vector<EdgeId>& edge_ids) {
  for (const auto& edge_id : edge_ids)
    if (id == edge_id)
      return true;
  return false;
}

std::string Vertex::to_json() const {
  std::string res;
  res = "{ \"id\": ";
  res += to_string(get_id()) + ", \"edge_ids\": [";
  for (const auto& edge_id : edges_ids_) {
    res += to_string(edge_id);
    res += ", ";
  }
  if (edges_ids_.size() > 0) {
    res.pop_back();
    res.pop_back();
  }
  res += "] }";
  return res;
}

std::string Graph::to_json() const {
  std::string res;
  res = "{ \"depth\": ";
  res += to_string(depth_);
  res += ", \"vertices\": [ ";
  for (const auto& vertex : vertices_) {
    res += vertex.to_json();
    res += ", ";
  }
  if (vertices_.size()) {
    res.pop_back();
    res.pop_back();
  }
  res += " ], \"edges\": [ ";
  for (const auto& edge : edges_) {
    res += edge.to_json();
    res += ", ";
  }
  if (edges_.size() > 0) {
    res.pop_back();
    res.pop_back();
  }
  res += " ] }\n";
  return res;
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
                             const bool& initialization) {
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

  const Color color = [&initialization, &diff, &from_vertex_id,
                       &to_vertex_id]() {
    if (initialization)
      return Color::Gray;
    else if (from_vertex_id == to_vertex_id)
      return Color::Green;
    else if (diff == 0)
      return Color::Blue;
    else if (diff == 1)
      return Color::Yellow;
    else if (diff == 2)
      return Color::Red;
    else
      return Color::Gray;
  }();

  const auto& new_edge = edges_.emplace_back(from_vertex_id, to_vertex_id,
                                             get_next_edge_id(), color);
  vertices_[from_vertex_id].add_edge_id(new_edge.id);
  if (from_vertex_id != to_vertex_id)
    vertices_[to_vertex_id].add_edge_id(new_edge.id);
}

}  // namespace uni_cpp_practice
