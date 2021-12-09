#include <algorithm>
#include <cassert>
#include <stdexcept>

#include "graph.hpp"

namespace uni_course_cpp {
void Vertex::add_edge_id(const EdgeId& id) {
  assert(!has_edge_id(id) && "Edge id already exists");
  edge_ids_.push_back(id);
}

const std::vector<EdgeId>& Vertex::get_edge_ids() const {
  return edge_ids_;
}

bool Vertex::has_edge_id(const EdgeId& id) const {
  if (edge_ids_.empty())
    return false;
  if (std::find(edge_ids_.begin(), edge_ids_.end(), id) != edge_ids_.end())
    return true;
  return false;
}

bool Graph::are_connected(const VertexId& vertex1_id,
                          const VertexId& vertex2_id) const {
  assert(has_vertex(vertex1_id));
  assert(has_vertex(vertex2_id));

  if (vertex1_id != vertex2_id) {
    for (const auto& edge1_id : get_vertex(vertex1_id).get_edge_ids())
      for (const auto& edge2_id : get_vertex(vertex2_id).get_edge_ids())
        if (edge1_id == edge2_id)
          return true;
  } else {
    for (const auto& edge_id : get_vertex(vertex1_id).get_edge_ids()) {
      const Edge& edge = get_edge(edge_id);
      if (edge.vertex1_id == edge.vertex2_id)
        return true;
    }
  }
  return false;
}

bool Graph::has_vertex(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == vertex_id)
      return true;
  return false;
}

const Edge& Graph::get_edge(const EdgeId& edge_id) const {
  for (const auto& edge : edges_)
    if (edge.id == edge_id)
      return edge;
  throw std::runtime_error("Edge with such id doesn't exist");
}

const Vertex& Graph::get_vertex(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == vertex_id)
      return vertex;
  throw std::runtime_error("Vertex with such id doesn't exist");
}

Vertex& Graph::get_vertex(const VertexId& vertex_id) {
  const auto& const_this = *this;
  return const_cast<Vertex&>(const_this.get_vertex(vertex_id));
}

void Graph::add_edge(const VertexId& vertex1_id, const VertexId& vertex2_id) {
  assert(has_vertex(vertex1_id));
  assert(has_vertex(vertex2_id));
  assert(!are_connected(vertex1_id, vertex2_id));

  const auto less_vertex_id = std::min(vertex1_id, vertex2_id);
  const auto greater_vertex_id = std::max(vertex1_id, vertex2_id);

  const auto color = calculate_edge_color(less_vertex_id, greater_vertex_id);

  if (color == Edge::Color::Gray)
    gray_edges_count_++;
  else if (color == Edge::Color::Green)
    green_edges_count_++;
  else if (color == Edge::Color::Yellow)
    yellow_edges_count_++;
  else
    red_edges_count_++;

  const auto& new_edge = edges_.emplace_back(less_vertex_id, greater_vertex_id,
                                             get_max_edge_id(), color);

  Vertex& vertex1 = get_vertex(less_vertex_id);
  vertex1.add_edge_id(new_edge.id);
  if (less_vertex_id != greater_vertex_id) {
    Vertex& vertex2 = get_vertex(greater_vertex_id);
    vertex2.add_edge_id(new_edge.id);
  }

  if (color == Edge::Color::Gray) {
    set_vertex_depth(greater_vertex_id, vertex1.depth + 1);
  }
}

const VertexId& Graph::add_vertex() {
  const auto& new_vertex = vertices_.emplace_back(get_max_vertex_id());
  if (depth_map_.size() == 0)
    depth_map_.push_back({new_vertex.id});
  else
    depth_map_[0].push_back(new_vertex.id);
  return new_vertex.id;
}

const std::vector<Edge>& Graph::get_edges() const {
  return edges_;
}

const std::vector<Vertex>& Graph::get_vertices() const {
  return vertices_;
}

const std::vector<VertexId>& Graph::get_vertex_ids_in_depth(int depth) const {
  return depth_map_[depth];
}

const std::vector<std::vector<VertexId>>& Graph::get_depth_map() const {
  return depth_map_;
}

const int Graph::get_depth() const {
  return depth_map_.size() - 1;
}

const int Graph::get_edges_count_by_color(const Edge::Color color) const {
  if (color == Edge::Color::Gray)
    return gray_edges_count_;
  else if (color == Edge::Color::Green)
    return green_edges_count_;
  else if (color == Edge::Color::Yellow)
    return yellow_edges_count_;
  else
    return red_edges_count_;
};

void Graph::set_vertex_depth(const VertexId& vertex_id, int depth) {
  Vertex& vertex = get_vertex(vertex_id);
  vertex.depth = depth;

  while (depth_map_.size() <= depth) {
    depth_map_.push_back({});
  }
  depth_map_[depth].push_back(vertex_id);

  for (auto vertex_id_on_zero_depth = depth_map_[0].begin();
       vertex_id_on_zero_depth != depth_map_[0].end();
       vertex_id_on_zero_depth++) {
    if (*vertex_id_on_zero_depth == vertex.id) {
      depth_map_[0].erase(vertex_id_on_zero_depth);
      break;
    }
  }
}

const Edge::Color Graph::calculate_edge_color(
    const VertexId& vertex1_id,
    const VertexId& vertex2_id) const {
  if (vertex1_id == vertex2_id) {
    return Edge::Color::Green;
  }

  const auto& vertex1 = get_vertex(std::min(vertex1_id, vertex2_id));
  const auto& vertex2 = get_vertex(std::max(vertex1_id, vertex2_id));
  auto depth_difference = std::abs(vertex1.depth - vertex2.depth);

  if ((vertex1.get_edge_ids().size() == 0) ||
      (vertex2.get_edge_ids().size() == 0)) {
    return Edge::Color::Gray;
  } else if (depth_difference == 1) {
    return Edge::Color::Yellow;
  } else if (depth_difference == 2) {
    return Edge::Color::Red;
  } else {
    throw std::runtime_error("Can't calculate edge color");
  }
}

const EdgeId Graph::get_max_edge_id() {
  const auto id = edge_id_max_;
  edge_id_max_++;
  return id;
}

const VertexId Graph::get_max_vertex_id() {
  const auto id = vertex_id_max_;
  vertex_id_max_++;
  return id;
}

std::vector<std::pair<Edge::Color, int>> Graph::get_color_edge_count_pairs()
    const {
  std::vector<std::pair<uni_course_cpp::Edge::Color, int>> color_counts = {
      {uni_course_cpp::Edge::Color::Gray,
       get_edges_count_by_color(uni_course_cpp::Edge::Color::Gray)},
      {uni_course_cpp::Edge::Color::Green,
       get_edges_count_by_color(uni_course_cpp::Edge::Color::Green)},
      {uni_course_cpp::Edge::Color::Yellow,
       get_edges_count_by_color(uni_course_cpp::Edge::Color::Yellow)},
      {uni_course_cpp::Edge::Color::Red,
       get_edges_count_by_color(uni_course_cpp::Edge::Color::Red)}};
  return color_counts;
}

}  // namespace uni_course_cpp
