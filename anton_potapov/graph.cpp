#include <iostream>

#include <algorithm>
#include <cassert>
#include <random>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "graph_traverser.hpp"

namespace uni_cource_cpp {
std::string get_edge_color_string(const EdgeColor& color) {
  switch (color) {
    case EdgeColor::Gray:
      return "gray";
    case EdgeColor::Green:
      return "green";
    case EdgeColor::Blue:
      return "blue";
    case EdgeColor::Yellow:
      return "yellow";
    case EdgeColor::Red:
      return "red";
  }
}

Vertex::Vertex(const VertexId& vertex_id) : id(vertex_id) {}

Edge::Edge(const EdgeId& edge_id,
           const VertexId& vertex1,
           const VertexId& vertex2,
           const EdgeColor& edge_color)
    : vertex1_id(vertex1),
      vertex2_id(vertex2),
      color(edge_color),
      id(edge_id) {}

const std::set<EdgeId>& Vertex::connected_edges() const {
  return connected_edges_;
}

bool Vertex::has_edge_id(const EdgeId& edge_id) const {
  return connected_edges_.find(edge_id) != connected_edges_.end();
}

void Vertex::add_edge(const EdgeId& edge_id) {
  assert(!has_edge_id(edge_id) && "edge that is to be added already exists");
  connected_edges_.insert(edge_id);
}

const Edge& Graph::get_edge(const EdgeId& id) const {
  return edges_.at(id);
}

Edge& Graph::get_edge(const EdgeId& id) {
  const auto& const_this = *this;
  return const_cast<Edge&>(const_this.get_edge(id));
}

const std::map<EdgeId, Edge>& Graph::edges() const {
  return edges_;
}

const VertexId& Graph::get_root_vertex_id() const {
  return vertices_.begin()->first;
}

bool Graph::is_vertex_exists(const VertexId& vertex_id) const {
  return vertices_.find(vertex_id) != vertices_.end();
}

bool Graph::is_connected(const VertexId& vertex1_id,
                         const VertexId& vertex2_id) const {
  assert(is_vertex_exists(vertex1_id) && "Vertex 1 doesn't exist");
  assert(is_vertex_exists(vertex2_id) && "Vertex 2 doesn't exist");
  if (vertex1_id == vertex2_id) {
    for (const auto& vertex_edge_id : connected_edges(vertex1_id)) {
      const auto& vertex_edge = get_edge(vertex_edge_id);
      if (vertex_edge.vertex1_id == vertex_edge.vertex2_id) {
        return true;
      }
    }
    return false;
  } else {
    for (const auto& vertex1_edge_id : connected_edges(vertex1_id)) {
      if (vertices_.find(vertex2_id)->second.has_edge_id(vertex1_edge_id)) {
        return true;
      }
    }
    return false;
  }
}

VertexId Graph::add_vertex() {
  const VertexId new_vertex_id = get_next_vertex_id();
  vertices_.emplace(new_vertex_id, new_vertex_id);
  return new_vertex_id;
}

EdgeId Graph::add_edge(const VertexId& vertex1_id,
                       const VertexId& vertex2_id,
                       const EdgeColor& edge_color) {
  assert(is_vertex_exists(vertex1_id) && "Vertex 1 doesn't exist");
  assert(is_vertex_exists(vertex2_id) && "Vertex 2 doesn't exist");
  assert(!is_connected(vertex1_id, vertex2_id) && "Vertices already connected");
  assert(new_edge_color_is_correct(vertex1_id, vertex2_id, edge_color) &&
         "the new edge's color is incorrect");

  const EdgeId new_edge_id = get_next_edge_id();
  edges_.emplace(new_edge_id,
                 Edge(new_edge_id, vertex1_id, vertex2_id, edge_color));

  vertices_.at(vertex1_id).add_edge(new_edge_id);
  if (vertex1_id != vertex2_id) {
    vertices_.at(vertex2_id).add_edge(new_edge_id);
  }

  is_depth_dirty_ = true;
  if (updated_depth_ != INIT_DEPTH) {
    updated_depth_ =
        std::min(updated_depth_, get_new_depth(vertex1_id, vertex2_id));
  }
  return new_edge_id;
}

const std::set<EdgeId> Graph::connected_edges(const VertexId& vertex_id) const {
  return vertices_.at(vertex_id).connected_edges();
}

int Graph::edge_with_color_cnt(const EdgeColor& edge_color) const {
  int ans = 0;
  for (const auto& [edge_id, edge] : edges_) {
    if (edge.color == edge_color) {
      ++ans;
    }
  }
  return ans;
}

VertexId Graph::get_next_vertex_id() {
  return next_vertex_id_++;
}
EdgeId Graph::get_next_edge_id() {
  return next_edge_id_++;
}

bool Graph::new_edge_color_is_correct(const VertexId& vertex1_id,
                                      const VertexId& vertex2_id,
                                      const EdgeColor& color) const {
  switch (color) {
    case EdgeColor::Gray:
      return get_vertex(vertex1_id).connected_edges().empty() ||
             get_vertex(vertex2_id).connected_edges().empty();
    case EdgeColor::Green:
      return vertex1_id == vertex2_id;
    case EdgeColor::Blue:
      return get_vertex(vertex1_id).depth == get_vertex(vertex2_id).depth;
    case EdgeColor::Yellow:
      return (std::abs(get_vertex(vertex1_id).depth -
                       get_vertex(vertex2_id).depth) == 1) &&
             !is_connected(vertex1_id, vertex2_id);
    case EdgeColor::Red:
      return std::abs(get_vertex(vertex1_id).depth -
                      get_vertex(vertex2_id).depth) == 2;
  }
}

VertexId get_random_vertex_id(const std::set<VertexId>& vertex_id_set) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, (int)vertex_id_set.size() - 1);
  auto vertex_id_set_it = vertex_id_set.begin();
  std::advance(vertex_id_set_it, dist(gen));
  return *vertex_id_set_it;
}

int Graph::get_new_depth(const VertexId& vertex1_id,
                         const VertexId& vertex2_id) const {
  const auto vertex1_depth = vertices_.at(vertex1_id).depth;
  const auto vertex2_depth = vertices_.at(vertex2_id).depth;
  if (vertex1_depth == INIT_DEPTH && vertex2_depth == INIT_DEPTH) {
    return INIT_DEPTH;
  }
  if (vertex1_depth == INIT_DEPTH) {
    return vertex2_depth + 1;
  } else if (vertex2_depth == INIT_DEPTH) {
    return vertex1_depth + 1;
  } else {
    return std::min(vertex1_depth, vertex2_depth) + 1;
  }
}

void Graph::update_vertices_depth() {
  if (!is_depth_dirty_) {
    return;
  }
  const auto& depths = GraphTraverser::get_updated_depths(
      *this, vertices_at_depth_, updated_depth_);
  if (updated_depth_ == INIT_DEPTH) {
    vertices_at_depth_.clear();
  } else {
    vertices_at_depth_.erase(vertices_at_depth_.find(updated_depth_),
                             vertices_at_depth_.end());
  }
  for (const auto& [vertex_id, depth] : depths) {
    vertices_.at(vertex_id).depth = depth;
    vertices_at_depth_[depth].insert(vertex_id);
  }
  is_depth_dirty_ = false;
}

Vertex Graph::get_vertex(const VertexId& id) const {
  Vertex vertex_copy = vertices_.at(id);
  if (is_depth_dirty_) {
    vertex_copy.depth = GraphTraverser::get_updated_vertex_depth(
        *this, vertices_at_depth_, id, updated_depth_);
  }
  return vertex_copy;
}

Vertex& Graph::get_vertex(const VertexId& id) {
  update_vertices_depth();
  return vertices_.at(id);
}

int Graph::max_depth() {
  update_vertices_depth();
  return vertices_at_depth_.size() - 1;
}

int Graph::max_depth() const {
  if (is_depth_dirty_) {
    return GraphTraverser::get_max_depth(*this, vertices_at_depth_,
                                         updated_depth_);
  } else {
    return vertices_at_depth_.size() - 1;
  }
}

std::map<VertexId, Vertex> Graph::vertices() const {
  auto vertices_updated_copy = vertices_;
  const auto updated_depths = GraphTraverser::get_updated_depths(
      *this, vertices_at_depth_, updated_depth_);
  for (auto& [vertex_id, vertex] : vertices_updated_copy) {
    vertex.depth = updated_depths.at(vertex_id);
  }
  return vertices_updated_copy;
}

const std::map<VertexId, Vertex>& Graph::vertices() {
  update_vertices_depth();
  return vertices_;
}

std::set<VertexId> Graph::get_vertices_at_depth(int depth) const {
  if (is_depth_dirty_) {
    return GraphTraverser::get_updated_vertices_at_depth(
        *this, vertices_at_depth_, depth, INIT_DEPTH);
  } else {
    return vertices_at_depth_.at(depth);
  }
}

const std::set<VertexId>& Graph::get_vertices_at_depth(int depth) {
  update_vertices_depth();
  return vertices_at_depth_.at(depth);
}
}  // namespace uni_cource_cpp
