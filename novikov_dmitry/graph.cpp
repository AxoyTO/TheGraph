#include "graph.hpp"
#include <cassert>

namespace {
using Vertex = uni_cpp_practice::Vertex;
using Edge = uni_cpp_practice::Edge;
bool check_gray_valid(const Vertex& first_vertex, const Vertex& second_vertex) {
  if (first_vertex.get_edge_ids().size() == 0 ||
      second_vertex.get_edge_ids().size() == 0) {  //только текущее ребро
    return true;
  }
  return false;
}

bool check_green_valid(const Vertex& first_vertex,
                       const Vertex& second_vertex) {
  if (first_vertex.depth == second_vertex.depth) {
    return true;
  }
  return false;
}

bool check_blue_valid(const Vertex& first_vertex, const Vertex& second_vertex) {
  if (first_vertex.depth == second_vertex.depth) {
    if (std::abs(first_vertex.id - second_vertex.id) == 1) {
      return true;
    }
  }
  return false;
}

bool check_yellow_valid(const Vertex& first_vertex,
                        const Vertex& second_vertex) {
  if (std::abs(first_vertex.depth - second_vertex.depth) == 1) {
    return true;
  }
  return false;
}

bool check_red_valid(const Vertex& first_vertex, const Vertex& second_vertex) {
  if (std::abs(first_vertex.depth - second_vertex.depth) == 2) {
    return true;
  }
  return false;
}

bool check_color_valid(const Vertex& first_vertex,
                       const Vertex& second_vertex,
                       const Edge::Color& color) {
  switch (color) {
    case Edge::Color::Gray:
      return check_gray_valid(first_vertex, second_vertex);
    case Edge::Color::Green:
      return check_green_valid(first_vertex, second_vertex);
    case Edge::Color::Blue:
      return check_blue_valid(first_vertex, second_vertex);
    case Edge::Color::Yellow:
      return check_yellow_valid(first_vertex, second_vertex);
    case Edge::Color::Red:
      return check_red_valid(first_vertex, second_vertex);
  }
}
}  // namespace

namespace uni_cpp_practice {
bool Vertex::has_edge_id(const EdgeId& new_edge_id) const {
  if (std::find(edge_ids_.begin(), edge_ids_.end(), new_edge_id) !=
      edge_ids_.end()) {
    return true;
  }
  return false;
}

void Vertex::add_edge_id(const EdgeId& new_edge_id) {
  assert(!has_edge_id(new_edge_id) && "Edge id already exists");
  edge_ids_.push_back(new_edge_id);
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

VertexId Graph::add_vertex() {
  const auto new_vertex_id = get_default_vertex_id();
  vertex_map_.insert({new_vertex_id, Vertex(new_vertex_id)});
  get_mutable_vertices_at_depth(DEFAULT_DEPTH).push_back(new_vertex_id);
  return new_vertex_id;
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id,
                     const Edge::Color& new_edge_color) {
  assert(has_vertex(from_vertex_id) && "Vertex doesn't exists");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exists");
  assert(!check_binding(from_vertex_id, to_vertex_id) &&
         "Vertices already binded");
  assert(check_color_valid(get_vertex(from_vertex_id), get_vertex(to_vertex_id),
                           new_edge_color) &&
         "Not valid color");
  const auto new_edge_id = get_default_edge_id();
  const auto new_edge =
      edge_map_.insert({new_edge_id, Edge(from_vertex_id, to_vertex_id,
                                          new_edge_id, new_edge_color)});
  get_mutable_vertex(from_vertex_id).add_edge_id(new_edge.first->first);
  if (from_vertex_id != to_vertex_id) {
    get_mutable_vertex(to_vertex_id).add_edge_id(new_edge.first->first);
  }
  if (new_edge_color == Edge::Color::Gray) {
    set_vertex_depth(from_vertex_id, to_vertex_id);
  }
}

bool Graph::check_binding(const VertexId& from_vertex_id,
                          const VertexId& to_vertex_id) const {
  assert(has_vertex(from_vertex_id) && "Vertex doesn't exists");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exists");
  const auto& from_vertex = get_vertex(from_vertex_id);
  const auto& to_vertex = get_vertex(to_vertex_id);
  if (from_vertex_id == to_vertex_id) {
    for (const auto& from_vertex_edge_id : from_vertex.get_edge_ids()) {
      const auto& binded_vertices =
          get_edge(from_vertex_edge_id).get_binded_vertices();
      if (binded_vertices.first == binded_vertices.second) {
        return true;
      }
    }
  } else {
    for (const auto& from_vertex_edge_id : from_vertex.get_edge_ids()) {
      if (to_vertex.has_edge_id(from_vertex_edge_id)) {
        return true;
      }
    }
  }
  return false;
}

int Graph::count_edges_of_color(const Edge::Color& color) const {
  int count = 0;
  for (const auto& [edge_id, edge] : get_edge_map()) {
    if (edge.color == color) {
      ++count;
    }
  }
  return count;
}

Depth Graph::get_depth() const {
  return (depth_map_.size() > DEFAULT_DEPTH) ? (depth_map_.size() - 1)
                                             : DEFAULT_DEPTH;
}

const std::vector<VertexId>& Graph::get_vertices_at_depth(
    const Depth& depth) const {
  assert(depth <= get_depth() && "Depth level doesn't exist");
  return depth_map_.at(depth);
}

const Vertex& Graph::get_vertex(const VertexId& id) const {
  assert(has_vertex(id) && "Vertex doesn't exist");
  return vertex_map_.at(id);
}

const Edge& Graph::get_edge(const EdgeId& id) const {
  assert(has_edge(id) && "Edge doesn't exist");
  return edge_map_.at(id);
}

void Graph::set_vertex_depth(const VertexId& from_vertex_id,
                             const VertexId& to_vertex_id) {
  assert(has_vertex(from_vertex_id) && "Vertex doesn't exists");
  assert(has_vertex(to_vertex_id) && "Vertex doesn't exists");
  assert(check_binding(from_vertex_id, to_vertex_id) &&
         "Vertices already binded");
  VertexId parent_vertex_id = std::min(from_vertex_id, to_vertex_id);
  VertexId son_vertex_id = std::max(from_vertex_id, to_vertex_id);

  const auto new_son_vertex_depth = get_vertex(parent_vertex_id).depth + 1;

  auto& son_vertex = get_mutable_vertex(son_vertex_id);
  auto& depth_map_son_level = get_mutable_vertices_at_depth(son_vertex.depth);

  const auto vertex_iter_at_depth_map = std::remove(
      depth_map_son_level.begin(), depth_map_son_level.end(), son_vertex.id);
  depth_map_son_level.erase(vertex_iter_at_depth_map);
  son_vertex.depth = new_son_vertex_depth;

  if (depth_map_.size() <= new_son_vertex_depth) {
    depth_map_.push_back(std::vector<VertexId>({son_vertex.id}));
  } else {
    get_mutable_vertices_at_depth(new_son_vertex_depth)
        .push_back(son_vertex.id);
  }
}
}  // namespace uni_cpp_practice
