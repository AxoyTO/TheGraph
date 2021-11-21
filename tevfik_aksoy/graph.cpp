#include "graph.hpp"
#include <cassert>
#include <iostream>

namespace {
bool is_depth_valid(
    int depth,
    const std::vector<std::vector<uni_cpp_practice::VertexId>>& depth_map) {
  if (depth > depth_map.size())
    return false;
  if (depth_map[depth].empty())
    return false;
  return true;
}
}  // namespace

namespace uni_cpp_practice {

void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id, edge_ids_) && "Edge already exists in vertex!");
  edge_ids_.push_back(id);
}

const std::vector<EdgeId>& Vertex::get_edge_ids() const {
  return edge_ids_;
}

std::string color_to_string(const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return "gray";
    case Edge::Color::Green:
      return "green";
    case Edge::Color::Blue:
      return "blue";
    case Edge::Color::Yellow:
      return "yellow";
    case Edge::Color::Red:
      return "red";
  }
}

bool Graph::does_vertex_exist(const VertexId& id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == id) {
      return true;
    }
  return false;
}

VertexId Graph::insert_vertex() {
  const auto id = get_new_vertex_id();
  vertices_.emplace_back(id);
  if (id == 0) {
    depth_map_.emplace_back();
    depth_map_[0].push_back(id);
  }
  return id;
}

Edge::Color Graph::calculate_color_for_edge(const Vertex& source,
                                            const Vertex& destination) const {
  if (source.get_edge_ids().empty() || destination.get_edge_ids().empty()) {
    return Edge::Color::Gray;
  }
  if (source.id == destination.id)
    return Edge::Color::Green;
  if (source.depth == destination.depth) {
    for (int i = 0; i < depth_map_[source.depth].size() - 1; i++) {
      const auto first = depth_map_[source.depth][i];
      const auto second = depth_map_[source.depth][i + 1];
      if ((source.id == first && destination.id == second) ||
          (destination.id == first && source.id == second))
        return Edge::Color::Blue;
    }
  }
  if (source.depth == destination.depth - 1)
    return Edge::Color::Yellow;
  if (source.depth == destination.depth - 2)
    return Edge::Color::Red;

  throw std::runtime_error("Failed to calculate edge color");
}

Vertex& Graph::get_vertex(const VertexId& id) {
  for (auto& vertex : vertices_) {
    if (id == vertex.id)
      return vertex;
  }
  throw std::runtime_error("Vertex not found!");
}

void Graph::insert_edge(const VertexId& source_id,
                        const VertexId& destination_id) {
  assert(does_vertex_exist(source_id) && "Source vertex doesn't exist!");
  assert(does_vertex_exist(destination_id) &&
         "Destination vertex doesn't exist!");
  assert(!are_vertices_connected(source_id, destination_id) &&
         "Vertices are already connected!");
  const auto& source_vertex = get_vertex(source_id);
  const auto& destination_vertex = get_vertex(destination_id);
  const auto color =
      calculate_color_for_edge(source_vertex, destination_vertex);
  const int edge_id = get_new_edge_id();
  colored_edges_map_[color].push_back(edge_id);
  edges_.emplace_back(source_id, destination_id, edge_id, color);

  vertices_[source_id].add_edge_id(edge_id);
  if (color != Edge::Color::Green) {
    vertices_[destination_id].add_edge_id(edge_id);
    if (color == Edge::Color::Gray) {
      const auto depth = vertices_[source_id].depth + 1;
      vertices_[destination_id].depth = depth;
      if (depth_map_.size() == depth) {
        depth_map_.emplace_back();
      }
      depth_map_[depth].emplace_back(destination_id);
    }
  }
}

bool Graph::are_vertices_connected(const VertexId& source,
                                   const VertexId& destination) const {
  assert(does_vertex_exist(source) && "Source vertex doesn't exist!");
  assert(does_vertex_exist(destination) && "Destination vertex doesn't exist!");

  const auto& source_vertex_edges = vertices_[source].get_edge_ids();
  const auto& destination_vertex_edges = vertices_[destination].get_edge_ids();
  for (const auto& edge_of_source_vertex : source_vertex_edges)
    if (source == destination) {
      if (edges_[edge_of_source_vertex].source ==
          edges_[edge_of_source_vertex].destination)
        return true;
    } else {
      for (const auto& edge_of_destination_vertex : destination_vertex_edges)
        if (edge_of_source_vertex == edge_of_destination_vertex)
          return true;
    }
  return false;
}

const std::vector<EdgeId>& Graph::get_colored_edges(
    const Edge::Color& color) const {
  if (colored_edges_map_.find(color) == colored_edges_map_.end()) {
    static std::vector<EdgeId> empty_result;
    return empty_result;
  }
  return colored_edges_map_.at(color);
}

int Graph::depth() const {
  return depth_map_.size() - 1;
}

const std::vector<Vertex>& Graph::get_vertices() const {
  return vertices_;
}

const std::vector<Edge>& Graph::get_edges() const {
  return edges_;
}

const std::vector<VertexId>& Graph::get_vertices_in_depth(
    const VertexDepth& depth) const {
  assert(is_depth_valid(depth, depth_map_) && "Depth is not valid!");
  return depth_map_.at(depth);
}
}  // namespace uni_cpp_practice
