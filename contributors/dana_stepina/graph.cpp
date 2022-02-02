#include "graph.hpp"
#include <algorithm>
#include <cassert>
#include <stdexcept>

namespace uni_cource_cpp {
// VERTEX
Graph::Vertex::Vertex(const VertexId& vertex_id) : id(vertex_id) {}

void Graph::Vertex::add_edge_id(const EdgeId& edge_id) {
  assert(!has_edge_id(edge_id) && "Such an edge already exists!");
  edge_ids_.push_back(edge_id);
}

bool Graph::Vertex::has_edge_id(const EdgeId& edge_id) const {
  for (const auto& id : edge_ids_)
    if (edge_id == id)
      return true;
  return false;
}

// EDGE
Graph::Edge::Edge(const EdgeId& edge_id,
                  const VertexId& vertex_connection_start,
                  const VertexId& vertex_connection_end,
                  const Color& edge_color)
    : id(edge_id),
      vertex_start(vertex_connection_start),
      vertex_end(vertex_connection_end),
      color(edge_color) {}

// GRAPH
const Graph::Vertex& Graph::add_vertex() {
  if (depth_map_.empty())
    depth_map_.push_back({});

  const auto& new_vertex = vertices_.emplace_back(get_new_vertex_id());
  depth_map_.front().push_back(new_vertex.id);

  return new_vertex;
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  assert(has_vertex_id(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(has_vertex_id(to_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(!is_connected(from_vertex_id, to_vertex_id) &&
         "These vertices are already connected");

  auto& from_vertex = vertices_[from_vertex_id];
  auto& to_vertex = vertices_[to_vertex_id];
  const auto edge_color = get_edge_color(from_vertex, to_vertex);
  const auto& new_edge = edges_.emplace_back(get_new_edge_id(), from_vertex_id,
                                             to_vertex_id, edge_color);
  from_vertex.add_edge_id(new_edge.id);
  if (from_vertex_id != to_vertex_id)
    to_vertex.add_edge_id(new_edge.id);

  if (new_edge.color == Edge::Color::Grey)
    set_vertex_depth(to_vertex_id, from_vertex.depth + 1);

  edges_color_map_[edge_color].push_back(new_edge.id);
}

Graph::Edge::Color Graph::get_edge_color(const Vertex& from_vertex,
                                         const Vertex& to_vertex) {
  const auto& from_vertex_depth = from_vertex.depth;
  const auto& to_vertex_depth = to_vertex.depth;

  if (to_vertex.get_edge_ids().size() == 0)
    return Edge::Color::Grey;
  if (from_vertex.id == to_vertex.id)
    return Edge::Color::Green;
  if ((to_vertex_depth - from_vertex_depth == 1) &&
      (!is_connected(from_vertex.id, to_vertex.id)))
    return Edge::Color::Yellow;
  if (to_vertex_depth - from_vertex_depth == 2)
    return Edge::Color::Red;
  throw std::runtime_error("Can't determine color");
}

void Graph::set_vertex_depth(const VertexId& vertex_id, Depth depth) {
  vertices_[vertex_id].depth = depth;

  while (depth_map_.size() <= depth) {
    depth_map_.push_back({});
  }
  depth_map_[depth].push_back(vertex_id);

  for (auto vertex_id_on_zero_depth = depth_map_[0].begin();
       vertex_id_on_zero_depth != depth_map_[0].end();
       vertex_id_on_zero_depth++) {
    if (*vertex_id_on_zero_depth == vertex_id) {
      depth_map_[0].erase(vertex_id_on_zero_depth);
      break;
    }
  }
}

const std::vector<EdgeId>& Graph::get_colored_edges(
    const Edge::Color& color) const {
  if (edges_color_map_.find(color) == edges_color_map_.end()) {
    static std::vector<EdgeId> empty_result;
    return empty_result;
  }
  return edges_color_map_.at(color);
}

bool Graph::has_vertex_id(const VertexId& id) const {
  for (const auto& vertex : vertices_)
    if (id == vertex.id)
      return true;
  return false;
}

bool Graph::is_connected(const VertexId& from_vertex_id,
                         const VertexId& to_vertex_id) const {
  assert(has_vertex_id(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(has_vertex_id(to_vertex_id) &&
         "There is no such vertex in the graph.");

  for (const auto& edge_id_from_vertex :
       get_vertex(from_vertex_id).get_edge_ids())
    for (const auto& edge_id_to_vertex :
         get_vertex(to_vertex_id).get_edge_ids())
      if (from_vertex_id != to_vertex_id &&
          edge_id_from_vertex == edge_id_to_vertex)
        return true;
      else if (from_vertex_id == to_vertex_id) {
        const Edge& edge_vertex_start = get_edge(edge_id_from_vertex);
        if (edge_vertex_start.vertex_start == edge_vertex_start.vertex_end)
          return true;
      }

  return false;
}
}  // namespace uni_cource_cpp
