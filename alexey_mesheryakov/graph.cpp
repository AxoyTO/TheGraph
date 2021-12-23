#include "graph.hpp"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <utility>

using std::pair;
using std::vector;
namespace uni_course_cpp {

void Vertex::add_edge_id(const EdgeId& edge_id) {
  assert(!has_edge_id(edge_id) && "Edge id already exist");
  edge_ids_.push_back(edge_id);
}

bool Vertex::has_edge_id(const EdgeId& new_edge_id) const {
  for (const auto& edge_id : edge_ids_)
    if (edge_id == new_edge_id) {
      return true;
    }
  return false;
}

bool Graph::vertex_exist(const VertexId& id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == id)
      return true;
  return false;
}

bool Graph::edge_exist(const VertexId& first, const VertexId& second) const {
  if (first == second) {
    for (const auto& edge_id_from_vertex : get_vertex(first).get_edge_ids())
      if (get_edge(edge_id_from_vertex).vertex_ids.first ==
          get_edge(edge_id_from_vertex).vertex_ids.second)
        return true;
    return false;
  }
  for (const auto& edge_id_from_first_vertex : get_vertex(first).get_edge_ids())
    for (const auto& edge_id_from_second_vertex :
         get_vertex(second).get_edge_ids())
      if (edge_id_from_first_vertex == edge_id_from_second_vertex)
        return true;
  return false;
}

VertexId Graph::add_vertex() {
  VertexId vertex_id = get_new_vertex_id();
  if (levels_.size() == 0)
    levels_.push_back({vertex_id});
  else
    levels_[0].emplace_back(vertex_id);
  vertices_.emplace_back(vertex_id);
  return vertex_id;
}

void Graph::add_edge(const VertexId& first_id, const VertexId& second_id) {
  assert(vertex_exist(first_id) && "Source vertex id doesn't exist");
  assert(vertex_exist(second_id) && "Destination vertex id doesn't exist");
  assert(!edge_exist(first_id, second_id) && "Such edge already exist");
  const Edge::Color color = calculate_color(first_id, second_id);
  const auto& new_edge = edges_.emplace_back(
      pair<VertexId, VertexId>{first_id, second_id}, get_new_edge_id(), color);
  get_vertex(first_id).add_edge_id(new_edge.id);
  if (first_id != second_id)
    get_vertex(second_id).add_edge_id(new_edge.id);
  if (color == Edge::Color::Gray)
    update_vertex_depth(first_id, second_id);
}
const vector<VertexId>& Graph::get_vertex_ids_at_depth(
    const Depth& depth) const {
  assert(depth < levels_.size() && "Depth out of range");
  assert(depth >= 0 && "Depth out of range");
  return levels_[depth];
}
void Graph::update_vertex_depth(const VertexId& from_vertex_id,
                                const VertexId& to_vertex_id) {
  const VertexId parent_vertex_id = std::min(from_vertex_id, to_vertex_id);
  const VertexId son_vertex_id = std::max(from_vertex_id, to_vertex_id);
  const auto new_son_depth = get_vertex(parent_vertex_id).depth + 1;
  get_vertex(son_vertex_id).depth = new_son_depth;
  for (auto iter = levels_[0].begin(); iter != levels_[0].end(); ++iter) {
    if (*iter == son_vertex_id) {
      levels_[0].erase(iter);
      break;
    }
  }
  if (new_son_depth >= levels_.size()) {
    levels_.push_back(std::vector<VertexId>({son_vertex_id}));
  } else {
    levels_[new_son_depth].push_back(son_vertex_id);
  }
}

const Vertex& Graph::get_vertex(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_)
    if (vertex.id == vertex_id)
      return vertex;
  throw std::runtime_error("Vertex doesn't exist");
}
const Edge& Graph::get_edge(const EdgeId& edge_id) const {
  for (const auto& edge : edges_)
    if (edge.id == edge_id)
      return edge;
  throw std::runtime_error("Edge doesn't exist");
}
Vertex& Graph::get_vertex(const VertexId& vertex_id) {
  const auto& const_this = *this;
  return const_cast<Vertex&>(const_this.get_vertex(vertex_id));
}

Edge& Graph::get_edge(const EdgeId& edge_id) {
  const auto& const_this = *this;
  return const_cast<Edge&>(const_this.get_edge(edge_id));
}

Edge::Color Graph::calculate_color(const VertexId& first_id,
                                   const VertexId& second_id) {
  if (get_vertex(first_id).get_edge_ids().size() == 0 ||
      get_vertex(second_id).get_edge_ids().size() == 0)
    return Edge::Color::Gray;
  const Depth& first_depth = get_vertex(first_id).depth;
  const Depth& second_depth = get_vertex(second_id).depth;
  if (first_depth == second_depth) {
    if (first_id != second_id)
      return Edge::Color::Blue;
    else
      return Edge::Color::Green;
  }
  if (abs(first_depth - second_depth) == 1)
    return Edge::Color::Yellow;
  if (abs(first_depth - second_depth) == 2)
    return Edge::Color::Red;
  throw std::runtime_error("Can't calculate color");
}
}  // namespace uni_course_cpp
