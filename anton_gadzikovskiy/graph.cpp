#include "graph.hpp"
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace uni_cource_cpp {

using std::vector;

Graph::VertexId Graph::add_vertex() {
  if (vertices_on_depth_.empty()) {
    vertices_on_depth_.push_back(vector<VertexId>());
  }
  const VertexId new_vertex_id = get_new_vertex_id();
  vertices_.emplace(new_vertex_id, new_vertex_id);
  vertices_on_depth_[0].push_back(new_vertex_id);
  return new_vertex_id;
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  const EdgeId new_edge_id = get_new_edge_id();
  const auto& new_edge_color = define_edge_color(from_vertex_id, to_vertex_id);
  edges_.emplace(new_edge_id, Edge(new_edge_id, from_vertex_id, to_vertex_id,
                                   new_edge_color));
  colored_edge_ids_[new_edge_color].push_back(new_edge_id);
  get_vertex(from_vertex_id).add_edge_id(new_edge_id);
  if (new_edge_color != Edge::Color::Green) {
    get_vertex(to_vertex_id).add_edge_id(new_edge_id);
  }
  if (new_edge_color == Edge::Color::Gray) {
    const auto new_vertex_depth = get_vertex(from_vertex_id).depth + 1;
    get_vertex(to_vertex_id).depth = new_vertex_depth;
    if (vertices_on_depth_.size() - 1 < new_vertex_depth) {
      vertices_on_depth_.push_back(vector<VertexId>());
    }
    vertices_on_depth_[new_vertex_depth].push_back(to_vertex_id);
    for (auto vertex_id = vertices_on_depth_.at(0).begin();
         vertex_id != vertices_on_depth_[0].end(); vertex_id++) {
      if (to_vertex_id == *vertex_id) {
        vertices_on_depth_[0].erase(vertex_id);
        break;
      }
    }
  }
}

const std::vector<Graph::EdgeId>& Graph::get_colored_edge_ids(
    const Edge::Color& color) const {
  if (colored_edge_ids_.find(color) == colored_edge_ids_.end()) {
    static const std::vector<Graph::EdgeId> empty_result;
    return empty_result;
  }
  return colored_edge_ids_.at(color);
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  if (from_vertex_id != to_vertex_id) {
    for (const auto& edge_id_1 : get_vertex_edge_ids(from_vertex_id)) {
      for (const auto& edge_id_2 : get_vertex_edge_ids(to_vertex_id)) {
        if (edge_id_1 == edge_id_2) {
          return true;
        }
      }
    }
  } else {
    for (const auto& edge_id : get_vertex_edge_ids(from_vertex_id)) {
      if (get_edge(edge_id).from_vertex_id == get_edge(edge_id).to_vertex_id) {
        return true;
      }
    }
  }
  return false;
}

Graph::Edge::Color Graph::define_edge_color(
    const VertexId& from_vertex_id,
    const VertexId& to_vertex_id) const {
  const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
  if (from_vertex_id == to_vertex_id) {
    return Edge::Color::Green;
  }
  if (get_vertex_edge_ids(to_vertex_id).size() == 0) {
    return Edge::Color::Gray;
  }
  if (to_vertex_depth - from_vertex_depth == 1 &&
      !is_connected(from_vertex_id, to_vertex_id)) {
    return Edge::Color::Yellow;
  }
  if (to_vertex_depth - from_vertex_depth == 2) {
    return Edge::Color::Red;
  }
  throw std::runtime_error("Failed to determine color");
}

}  // namespace uni_cource_cpp
