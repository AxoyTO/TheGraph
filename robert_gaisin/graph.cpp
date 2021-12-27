#include "graph.hpp"
#include <cassert>
#include <iostream>
#include <random>

namespace {
using uni_course_cpp::Edge;
using uni_course_cpp::EdgeColor;

Edge::Duration determine_duration(const EdgeColor& color) {
  std::random_device rd;
  std::mt19937 mersenne(rd());
  switch (color) {
    case EdgeColor::Gray:
      return mersenne() % 2 + 1;
    case EdgeColor::Green:
      return mersenne() % 2 + 1;
    case EdgeColor::Blue:
      return 1000;
    case EdgeColor::Yellow:
      return mersenne() % 3 + 1;
    case EdgeColor::Red:
      return mersenne() % 3 + 2;
  }
  throw std::runtime_error("Color error");
}
}  // namespace
namespace uni_course_cpp {
bool Vertex::has_edge_id(const EdgeId& id) const {
  for (const auto& edge_id : edge_ids_)
    if (id == edge_id)
      return true;
  return false;
}

void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id) && "Edge doesn't exist");
  edge_ids_.push_back(id);
}

Edge::Edge(const EdgeId& new_id,
           const VertexId& begin_vertex,
           const VertexId& end_vertex,
           const EdgeColor _color)
    : id(new_id),
      begin(begin_vertex),
      end(end_vertex),
      color(_color),
      duration_(determine_duration(_color)) {}

Vertex& Graph::vertex(const VertexId& id) {
  for (auto& vertex : vertices_) {
    if (vertex.id == id)
      return vertex;
  }
  throw std::runtime_error("Vertex doesn't exist");
}

const Vertex& Graph::get_vertex(const VertexId& id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == id)
      return vertex;
  throw std::runtime_error("Vertex with such id doesn't exist");
}

const Edge& Graph::get_edge(const EdgeId& edge_id) const {
  for (const auto& edge : edges_)
    if (edge.id == edge_id)
      return edge;
  throw std::runtime_error("Edge with such id doesn't exist");
}

void Graph::add_id_to_depth_map(const VertexId& begin, const VertexId& end) {
  const size_t depth_for_new_vertex = get_vertex(begin).depth + 1;
  assert(!(depth_for_new_vertex > depth_map_.size()) &&
         "Error add id to depth map");
  if (depth_for_new_vertex == depth_map_.size()) {
    depth_map_.push_back({end});
  } else {
    depth_map_[depth_for_new_vertex].push_back(end);
  }
  vertex(end).depth = depth_for_new_vertex;
}

bool Graph::has_vertex(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_) {
    if (vertex_id == vertex.id)
      return true;
  }
  return false;
}

bool Graph::is_connected(const VertexId& begin, const VertexId& end) const {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  for (const EdgeId& edge_num : get_vertex(begin).edge_ids()) {
    if (get_edge(edge_num).begin == end || get_edge(edge_num).end == end) {
      if (begin != end || edges_[edge_num].color == EdgeColor::Green)
        return true;
    }
  }
  return false;
}

VertexId Graph::add_vertex() {
  auto vertex_id = next_vertex_id();
  vertices_.emplace_back(vertex_id);
  if (depth_map_.empty()) {
    depth_map_.push_back({vertex_id});
  }
  return vertex_id;
}

void Graph::add_edge(const VertexId& begin,
                     const VertexId& end,
                     const EdgeColor& color) {
  assert(has_vertex(begin) && "Vertex doesn't exist");
  assert(has_vertex(end) && "Vertex doesn't exist");
  assert(!(is_connected(begin, end)) && "Vertices already connected");
  const auto& edge = edges_.emplace_back(next_edge_id(), begin, end, color);
  vertex(begin).add_edge_id(edge.id);
  if (begin != end)
    vertex(end).add_edge_id(edge.id);
  if (color == EdgeColor::Gray)
    add_id_to_depth_map(begin, end);
  edges_color_map_[color].push_back(edge.id);
}

const std::vector<EdgeId>& Graph::get_colored_edges(
    const EdgeColor& color) const {
  if (edges_color_map_.find(color) == edges_color_map_.end()) {
    static std::vector<EdgeId> empty_result;
    return empty_result;
  }
  return edges_color_map_.at(color);
}

std::map<VertexId, Edge::Duration> Graph::get_linked_vertex_ids(
    const VertexId& vertex_id) const {
  const auto& vertex = get_vertex(vertex_id);
  std::map<VertexId, Edge::Duration> linked_ids;

  for (const auto& edge_id : vertex.edge_ids()) {
    const auto& edge = get_edge(edge_id);
    const auto linked_id = [&edge, &vertex_id]() {
      if (edge.color == EdgeColor::Green)
        return edge.begin;
      return edge.begin != vertex_id ? edge.begin : edge.end;
    }();
    linked_ids[linked_id] = edge.duration();
  }
  return linked_ids;
}

}  // namespace uni_course_cpp
