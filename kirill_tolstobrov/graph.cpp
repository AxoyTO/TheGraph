#include "graph.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>

namespace uni_cpp_practice {

const std::vector<EdgeId>& Vertex::get_edge_ids() const {
  return edge_ids_;
}
bool Vertex::check_edge_presence(const EdgeId& edge_id) const {
  for (const auto& id : edge_ids_) {
    if (edge_id == id) {
      return true;
    }
  }
  return false;
}
void Vertex::add_edge_id(const EdgeId& edge_id) {
  assert(!check_edge_presence(edge_id) &&
         "Attemptig to add added edge to vertex: Error.");
  edge_ids_.push_back(edge_id);
}

const std::vector<Vertex>& Graph::get_vertices() const {
  return vertices_;
}
const std::vector<Edge>& Graph::get_edges() const {
  return edges_;
}
const std::map<Edge::Color, std::vector<EdgeId>>& Graph::get_colors_map()
    const {
  return colors_map_;
}

const Vertex& Graph::get_vertex(const VertexId& id) const {
  for (const Vertex& vertex : vertices_) {
    if (vertex.id == id) {
      return vertex;
    }
  }
  throw std::runtime_error("Can't get vertex by id");
}

Vertex& Graph::get_vertex(const VertexId& id) {
  const auto& const_this = *this;
  return const_cast<Vertex&>(const_this.get_vertex(id));
}

VertexId Graph::add_new_vertex() {
  if (vertices_.size() == 0) {
    depths_map_.emplace_back();
  }
  const VertexId new_id = get_new_vertex_id();
  vertices_.emplace_back(new_id);
  depths_map_[0].push_back(new_id);
  return new_id;
}

bool Graph::check_vertex_existence(const VertexId& vertex_id) const {
  for (const auto& vertex : vertices_) {
    if (vertex_id == vertex.id) {
      return true;
    }
  }
  return false;
}

bool Graph::are_vertices_connected(const VertexId& id1,
                                   const VertexId& id2) const {
  assert(check_vertex_existence(id1) &&
         "Attemptig to access to nonexistent vertex: Error.");
  assert(check_vertex_existence(id2) &&
         "Attemptig to access to nonexistent vertex: Error.");
  if (connections_map_.find(id1) == connections_map_.end() ||
      connections_map_.find(id2) == connections_map_.end()) {
    return false;
  }
  for (const auto& edge_id : connections_map_.at(id1)) {
    if (((id1 == id2) &&
         (edges_[edge_id].vertex1_id == edges_[edge_id].vertex2_id)) ||
        ((id1 != id2) && (edges_[edge_id].vertex1_id == id2 ||
                          edges_[edge_id].vertex2_id == id2))) {
      return true;
    }
  }
  return false;
}

Edge::Color Graph::get_binding_color(const VertexId& id1,
                                     const VertexId& id2) const {
  assert(check_vertex_existence(id1) &&
         "Attemptig to access to nonexistent vertex: Error.");
  assert(check_vertex_existence(id2) &&
         "Attemptig to access to nonexistent vertex: Error.");

  if (id1 == id2) {
    return Edge::Color::Green;
  }

  const auto& vertex1 = get_vertex(id1);
  const auto& vertex2 = get_vertex(id2);

  if (vertex1.get_edge_ids().size() == 0 ||
      vertex2.get_edge_ids().size() == 0) {
    return Edge::Color::Grey;
  } else if (vertex1.depth - vertex2.depth == 0) {
    return Edge::Color::Blue;
  } else if (vertex1.depth - vertex2.depth == 1 ||
             vertex1.depth - vertex2.depth == -1) {
    return Edge::Color::Yellow;
  } else if (vertex1.depth - vertex2.depth == 2 ||
             vertex1.depth - vertex2.depth == -2) {
    return Edge::Color::Red;
  }

  throw std::runtime_error("Can't get binding color");
}

void Graph::bind_vertices(const VertexId& id1, const VertexId& id2) {
  assert(check_vertex_existence(id1) &&
         "Attemptig to access to nonexistent vertex: Error.");
  assert(check_vertex_existence(id2) &&
         "Attemptig to access to nonexistent vertex: Error.");
  assert(!are_vertices_connected(id1, id2) &&
         "Attemptig to connect connected vertices: Error.");
  Edge::Color edge_color = get_binding_color(id1, id2);

  if (edge_color == Edge::Color::Grey) {
    VertexId to_id = id1;
    VertexId from_id = id2;
    if (get_vertex(id2).get_edge_ids().size() == 0) {
      VertexId temp = from_id;
      from_id = to_id;
      to_id = temp;
    }
    get_vertex(to_id).depth = get_vertex(from_id).depth + 1;
    if (depths_map_.size() - 1 < get_vertex(to_id).depth) {
      depths_map_.emplace_back();
    }
    depths_map_[0].erase(
        std::remove(depths_map_[0].begin(), depths_map_[0].end(), to_id),
        depths_map_[0].end());
    depths_map_[depths_map_.size() - 1].push_back(to_id);
  }

  const auto& edge =
      edges_.emplace_back(get_new_edge_id(), id1, id2, edge_color);
  connections_map_[id1].push_back(edge.id);
  get_vertex(id1).add_edge_id(edge.id);
  if (id1 != id2) {
    connections_map_[id2].push_back(edge.id);
    get_vertex(id2).add_edge_id(edge.id);
  }
  colors_map_[edge_color].push_back(edge.id);
}

VertexId Graph::get_new_vertex_id() {
  return vertex_id_counter_++;
}
EdgeId Graph::get_new_edge_id() {
  return edge_id_counter_++;
}

}  // namespace uni_cpp_practice
