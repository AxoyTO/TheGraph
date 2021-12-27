#include "graph.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>

#include <unordered_map>

namespace uni_course_cpp {
const Edge& Graph::get_edge(EdgeId id) const {
  for (const auto& edge : edges_) {
    if (edge.get_id() == id) {
      return edge;
    }
  }
  throw std::runtime_error("Edge not found!\n");
}

const std::vector<Vertex>& Graph::get_vertices() const {
  return vertices_;
}

const std::vector<Edge>& Graph::get_edges() const {
  return edges_;
}

Vertex Graph::add_vertex() {
  const Vertex& vertex = vertices_.emplace_back(get_new_vertex_id());
  if (vertex.get_id() == 0) {
    depth_map_.push_back({0});
  } else {
    depth_map_[0].push_back(vertex.get_id());
  }
  vertices_depth_[vertex.get_id()] = 0;
  adjacency_list_[vertex.get_id()] = {};
  return vertex;
}

VertexId Graph::get_new_vertex_id() {
  return vertex_id_counter_++;
}
EdgeId Graph::get_new_edge_id() {
  return edge_id_counter_++;
}

Vertex& Graph::get_vertex(const VertexId& id) {
  for (auto& vertex : vertices_) {
    if (vertex.get_id() == id) {
      return vertex;
    }
  }
  throw std::runtime_error("Vertex not found!\n");
}

Graph::Depth Graph::get_vertex_depth(VertexId vertex_id) const {
  return vertices_depth_.at(vertex_id);
}

void Graph::add_edge(VertexId first_vertex_id, VertexId second_vertex_id) {
  Edge::Color color = get_edge_color(first_vertex_id, second_vertex_id);
  const auto& new_edge = edges_.emplace_back(get_new_edge_id(), first_vertex_id,
                                             second_vertex_id, color);

  auto& first_vertex = get_vertex(first_vertex_id);
  adjacency_list_[first_vertex_id].push_back(new_edge.get_id());
  auto& second_vertex = get_vertex(second_vertex_id);
  if (second_vertex.get_id() != first_vertex.get_id()) {
    adjacency_list_[second_vertex_id].push_back(new_edge.get_id());
  }
  if (color == Edge::Color::Grey) {
    vertices_depth_[second_vertex_id] = get_vertex_depth(first_vertex_id) + 1;
    if (depth_map_.size() < get_vertex_depth(first_vertex.get_id()) + 2) {
      depth_map_.push_back({second_vertex_id});
    } else {
      depth_map_[get_vertex_depth(first_vertex.get_id()) + 1].push_back(
          second_vertex_id);
    }
    auto depth_iterator =
        std::find(depth_map_[0].begin(), depth_map_[0].end(), second_vertex_id);
    if (depth_iterator != depth_map_[0].end()) {
      depth_map_[0].erase(depth_iterator);
    }
  }
}
Edge::Color Graph::get_edge_color(VertexId from_vertex_id,
                                  VertexId to_vertex_id) {
  const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
  if (from_vertex_id == to_vertex_id) {
    return Edge::Color::Green;
  }
  if (get_connected_edges_ids(to_vertex_id).size() == 0) {
    return Edge::Color::Grey;
  }

  if (to_vertex_depth - from_vertex_depth == 1 &&
      !(is_connected(from_vertex_id, to_vertex_id))) {
    return Edge::Color::Yellow;
  }
  if (to_vertex_depth - from_vertex_depth == 2) {
    return Edge::Color::Red;
  }
  throw std::runtime_error("Failed to determine color");
}

bool Graph::is_connected(VertexId from_vertex_id, VertexId to_vertex_id) const {
  for (const auto& connected_edge : adjacency_list_.at(from_vertex_id)) {
    if (get_edge(connected_edge).get_color() == Edge::Color::Green &&
        from_vertex_id == to_vertex_id) {
      return true;
    }
    const Edge& edge = get_edge(connected_edge);
    if ((edge.get_first_vertex_id() == to_vertex_id ||
         edge.get_second_vertex_id() == to_vertex_id) &&
        to_vertex_id != from_vertex_id) {
      return true;
    }
  }
  return false;
}
}  // namespace uni_course_cpp
