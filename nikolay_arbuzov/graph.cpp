#include "graph.hpp"

#include <algorithm>
#include <cassert>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace {
constexpr int k_new_vertex_depth = 0;
}

namespace uni_course_cpp {
Graph::Vertex& Graph::add_vertex() {
  VertexId new_vertex_id = get_new_vertex_id();
  set_vertex_depth(new_vertex_id, k_new_vertex_depth);
  adjacency_list_[new_vertex_id] = std::set<EdgeId>();
  return vertices_.emplace_back(new_vertex_id);
};

bool Graph::has_vertex(const VertexId& vertex_id) const {
  for (auto vertex : vertices_) {
    if (vertex.id == vertex_id) {
      return true;
    }
  }
  return false;
};

const Graph::Vertex& Graph::get_vertex(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_) {
    if (vertex.id == vertex_id) {
      return vertex;
    }
  }
  throw std::runtime_error("No such vertex");
}
const Graph::Edge& Graph::get_edge(const EdgeId& edge_id) const {
  for (const auto& edge : edges_) {
    if (edge.id == edge_id) {
      return edge;
    }
  }
  throw std::runtime_error("No such edge");
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  if (from_vertex_id == to_vertex_id) {
    const auto& edge_ids = adjacency_list_.at(from_vertex_id);
    for (const auto& edge_id : edge_ids) {
      if (get_edge(edge_id).color == Edge::Color::Green) {
        return true;
      }
    }
    return false;
  } else {
    const auto& from_edge_set = adjacency_list_.at(from_vertex_id);
    const auto& to_edge_set = adjacency_list_.at(to_vertex_id);
    std::set<EdgeId> intersection;
    std::set_intersection(from_edge_set.begin(), from_edge_set.end(),
                          to_edge_set.begin(), to_edge_set.end(),
                          std::inserter(intersection, intersection.begin()));
    if (intersection.size()) {
      return true;
    }
    return false;
  }
}

std::vector<Graph::VertexId> Graph::get_unconnected_vertex_ids(
    const Graph::VertexId& vertex_id) const {
  const auto& to_vertex_ids =
      get_vertex_ids_on_depth(get_vertex_depth(vertex_id) + 1);
  auto to_vertex_ids_no_neighbors = std::vector<Graph::VertexId>();

  for (const auto& not_neighbor_vertex_id : to_vertex_ids) {
    if (!is_connected(vertex_id, not_neighbor_vertex_id)) {
      to_vertex_ids_no_neighbors.push_back(not_neighbor_vertex_id);
    }
  }
  return to_vertex_ids_no_neighbors;
}

Graph::Edge::Color Graph::determine_color(const VertexId& from_vertex_id,
                                          const VertexId& to_vertex_id) const {
  const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
  const auto to_vertex_depth = get_vertex_depth(to_vertex_id);
  if (from_vertex_id == to_vertex_id) {
    return Graph::Edge::Color::Green;
  }
  if (adjacency_list_.at(to_vertex_id).size() == 0) {
    return Graph::Edge::Color::Grey;
  }
  if (to_vertex_depth - from_vertex_depth == 1 &&
      !is_connected(from_vertex_id, to_vertex_id)) {
    return Graph::Edge::Color::Yellow;
  }
  if (to_vertex_depth - from_vertex_depth == 2) {
    return Graph::Edge::Color::Red;
  }
  throw std::runtime_error("Failed to determine color");
}

Graph::Edge& Graph::add_edge(const VertexId& from_vertex_id,
                             const VertexId& to_vertex_id) {
  assert(has_vertex(from_vertex_id));
  assert(has_vertex(to_vertex_id));
  const auto& new_edge_color = determine_color(from_vertex_id, to_vertex_id);

  EdgeId new_edge_id = get_new_edge_id();

  color_map_[new_edge_color].push_back(new_edge_id);

  adjacency_list_[from_vertex_id].insert(new_edge_id);
  if (from_vertex_id != to_vertex_id)
    adjacency_list_[to_vertex_id].insert(new_edge_id);

  auto& new_edge = edges_.emplace_back(new_edge_id, new_edge_color,
                                       from_vertex_id, to_vertex_id);

  if (new_edge_color == Edge::Color::Grey) {
    const auto from_vertex_depth = get_vertex_depth(from_vertex_id);
    update_vertex_depth(to_vertex_id, from_vertex_depth + 1);
  }
  return new_edge;
};

bool Graph::has_edge(const EdgeId& edge_id) const {
  for (auto edge : edges_) {
    if (edge.id == edge_id) {
      return true;
    }
  }
  return false;
};

const std::vector<Graph::EdgeId>& Graph::get_color_edge_ids(
    const Graph::Edge::Color& color) const {
  if (color_map_.find(color) == color_map_.end()) {
    static auto empty_vector = std::vector<EdgeId>();
    return empty_vector;
  } else {
    return color_map_.at(color);
  }
}

std::string Graph::get_edges_distribution() const {
  std::stringstream result_string;
  result_string << "grey: " << get_color_edge_ids(Edge::Color::Grey).size()
                << ", ";
  result_string << "green: " << get_color_edge_ids(Edge::Color::Green).size()
                << ", ";
  result_string << "yellow: " << get_color_edge_ids(Edge::Color::Yellow).size()
                << ", ";
  result_string << "red: " << get_color_edge_ids(Edge::Color::Red).size();
  return result_string.str();
}

void Graph::set_vertex_depth(const VertexId& vertex_id, const Depth& depth) {
  vertices_depth_[vertex_id] = depth;
  if (!vertex_id)
    depth_map_.emplace_back();
  depth_map_[depth].push_back(vertex_id);
}

void Graph::update_vertex_depth(const VertexId& vertex_id, const Depth& depth) {
  if (!vertices_depth_[vertex_id]) {
    depth_map_[vertices_depth_[vertex_id]].erase(
        std::find(depth_map_[vertices_depth_[vertex_id]].begin(),
                  depth_map_[vertices_depth_[vertex_id]].end(), vertex_id));
  }
  if (depth_map_.size() - 1 < depth) {
    depth_map_.emplace_back();
  }
  vertices_depth_[vertex_id] = depth;
  depth_map_[depth].push_back(vertex_id);
}

}  // namespace uni_course_cpp
