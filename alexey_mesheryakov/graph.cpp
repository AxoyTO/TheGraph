#include "graph.hpp"
#include <assert.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string color_to_string(const Graph::Edge::Color& color) {
  switch (color) {
    case Graph::Edge::Color::Gray:
      return "gray";
    case Graph::Edge::Color::Green:
      return "green";
    case Graph::Edge::Color::Red:
      return "red";
    case Graph::Edge::Color::Blue:
      return "blue";
    case Graph::Edge::Color::Yellow:
      return "yellow";
  }
}

void Graph::Vertex::add_edge_id(const EdgeId& edge_id) {
  assert(!has_edge_id(edge_id) && "Edge id already exist");
  edge_ids_.push_back(edge_id);
}

bool Graph::Vertex::has_edge_id(const EdgeId& new_edge_id) const {
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

bool Graph::vertex_has_depth(const VertexId& id) const {
  for (const auto& level : levels_)
    for (const auto& vertex_id : level)
      if (vertex_id == id)
        return true;
  return false;
}

bool Graph::edge_exist(const VertexId& first, const VertexId& second) const {
  for (const auto& edge_id_from_first_vertex : vertices_[first].get_edge_ids())
    for (const auto& edge_id_from_second_vertex :
         vertices_[second].get_edge_ids())
      if (edge_id_from_first_vertex == edge_id_from_second_vertex)
        return true;
  return false;
}

VertexId Graph::add_vertex() {
  VertexId vertex_id = get_new_vertex_id();
  if (vertices_.size() == 0)
    levels_ = {{vertex_id}};
  vertices_.emplace_back(vertex_id);
  return vertex_id;
}

void Graph::add_edge(const VertexId& first,
                     const VertexId& second,
                     const Graph::Edge::Color& color) {
  assert(vertex_exist(first) && "Source vertex id doesn't exist");
  assert(vertex_exist(second) && "Destination vertex id doesn't exist");
  std::cout << "id: " << first << " " << second << std::endl;
  if (first != second)
    assert(!edge_exist(first, second) && "Such edge already exist");
  const auto& new_edge = edges_.emplace_back(
      pair<VertexId, VertexId>{first, second}, get_new_edge_id(), color);
  vertices_[first].add_edge_id(new_edge.id);
  if (first != second)
    vertices_[second].add_edge_id(new_edge.id);
  if (color == Graph::Edge::Color::Gray)
    set_vertex_depth(first, second);
}
const vector<VertexId>& Graph::get_vertices_at_depth(const Depth& depth) const {
  assert((depth <= depth_ && depth >= 0) && "Depth out of range");
  return levels_[depth];
}
void Graph::set_vertex_depth(const VertexId& from_vertex_id,
                             const VertexId& to_vertex_id) {
  VertexId parent_vertex_id = std::min(from_vertex_id, to_vertex_id);
  VertexId son_vertex_id = std::max(from_vertex_id, to_vertex_id);
  assert(vertex_has_depth(parent_vertex_id) && "Graph must be connected");
  const auto new_son_depth = get_vertex(parent_vertex_id).get_depth() + 1;
  set_vertex_depth_by_id(son_vertex_id, new_son_depth);
  if (new_son_depth > depth_) {
    depth_ = new_son_depth;
    levels_.push_back(std::vector<VertexId>({son_vertex_id}));
  } else {
    levels_[new_son_depth].push_back(son_vertex_id);
  }
}
