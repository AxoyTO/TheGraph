#include "graph.hpp"
#include <cassert>

// VERTEX
Vertex::Vertex(const VertexId& vertex_id) : id(vertex_id) {}

void Vertex::add_edge_id(const EdgeId& edge_id) {
  assert(!has_edge_id(edge_id) && "Such an edge already exists!");
  edge_ids_.push_back(edge_id);
}

bool Vertex::has_edge_id(const EdgeId& edge_id) const {
  for (const auto& id : edge_ids_)
    if (edge_id == id)
      return true;
  return false;
}

// EDGE
Edge::Edge(const EdgeId& edge_id,
           const VertexId& vertex_connection_start,
           const VertexId& vertex_connection_end,
           const Color& edge_color)
    : id(edge_id),
      vertex_start(vertex_connection_start),
      vertex_end(vertex_connection_end),
      color(edge_color) {}

// GRAPH
Vertex& Graph::add_vertex() {
  //добавить id новой вершины в дефолтную нулевую глубину
  if (depth_map_.empty())
    depth_map_.push_back({});
  depth_map_.front().push_back(get_vertex_id_counter());

  return vertices_.emplace_back(get_new_vertex_id());
}

void Graph::add_edge(const VertexId& from_vertex_id,
                     const VertexId& to_vertex_id) {
  //проверка данных
  assert(has_vertex_id(from_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(has_vertex_id(to_vertex_id) &&
         "There is no such vertex in the graph.");
  assert(!is_connected(from_vertex_id, to_vertex_id) &&
         "These vertices are already connected");

  // проверка порядка вершин?
  VertexId from_vertex_id_order = from_vertex_id;
  VertexId to_vertex_id_order = to_vertex_id;
  if (from_vertex_id > to_vertex_id) {
    from_vertex_id_order = to_vertex_id;
    to_vertex_id_order = from_vertex_id;
  }

  //добавление глубины вершины и пополнение карты глубины для серых граней
  if ((vertices_[to_vertex_id_order].depth == 0) && (to_vertex_id_order != 0)) {
    vertices_[to_vertex_id_order].depth =
        vertices_[from_vertex_id_order].depth + 1;
    set_vertex_depth(from_vertex_id_order, to_vertex_id_order);
  }

  //добавление ребра
  auto& new_edge =
      edges_.emplace_back(get_new_edge_id(), from_vertex_id, to_vertex_id_order,
                          set_edge_color(vertices_[from_vertex_id_order],
                                         vertices_[to_vertex_id_order]));
  vertices_[from_vertex_id_order].add_edge_id(new_edge.id);
  if (from_vertex_id_order != to_vertex_id_order)
    vertices_[to_vertex_id_order].add_edge_id(new_edge.id);
}

std::vector<VertexId> Graph::get_vertex_ids_at(const Depth& depth) const {
  std::vector<VertexId> vertex_ids;
  for (const auto& vertex : vertices_)
    if (vertex.depth == depth)
      vertex_ids.push_back(vertex.id);

  return vertex_ids;
}

Edge::Color Graph::set_edge_color(const Vertex& from_vertex,
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
  // if (to_vertex_depth - from_vertex_depth == 2)
  return Edge::Color::Red;
}

void Graph::set_vertex_depth(const VertexId& from_vertex_id,
                             const VertexId& to_vertex_id) {
  auto& depth_zero = depth_map_.front();
  const auto new_end =
      std::remove(depth_zero.begin(), depth_zero.end(), to_vertex_id);
  depth_zero.erase(new_end);

  //добавить id вершины в карту глубины
  if (depth_map_.size() - 1 == vertices_[from_vertex_id].depth)
    depth_map_.push_back({});
  depth_map_.back().push_back(to_vertex_id);
}

bool Graph::has_vertex_id(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_)
    if (vertex_id == vertex.id)
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
       vertices_[from_vertex_id].get_edge_ids())
    for (const auto& edge_id_to_vertex : vertices_[to_vertex_id].get_edge_ids())
      if ((edge_id_from_vertex == edge_id_to_vertex) &&
          (from_vertex_id != to_vertex_id))
        return true;
  return false;
}
